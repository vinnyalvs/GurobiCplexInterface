#include "stdafx.h"
#include "OptGurobi.h"

/**
CS-11 Asn 2
OptGurobi.cpp
Purpose: Implements Opt for Gurobi optimizer
Abbreviations:
var = variable
expr = Expressions
constr = Constraints
RINS =  Relaxation Induced Neighborhood Search
MIP =   Mixed Integer Programming
@author Vinicius Alves
@version 1.1 12/12/16
*/

//-----------------------------------------------------------------------


GurobiModel::GurobiModel(string name, GRBEnv env)
{
	//Its not necessary to keep the env, because when model is created it does his own copy of the env. See Referece Manual pg 139 "GRBEnv.set"

	model = new GRBModel(env);

	numConstraints = 0;
	numVars = 0;
	objective = 0;
	modelNeedUpdate = true;

}

GurobiModel::~GurobiModel()
{
	try {
		delete model;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}

}

GRBModel GurobiModel::getModel()
{
	return *model;
}


void GurobiModel::addVar(double upperbound, double obj, string name, string type, double lowerbound)
{

	try {
		char vtype;
		if (type == "float" || type == "double" || type == "continuos") {
			vtype = GRB_CONTINUOUS;
		}
		else if (type == "int") {
			vtype = GRB_INTEGER;
		}
		else if (type == "binary") {
			vtype = GRB_BINARY;
		}
		else if (type == "semicont") {
			vtype = GRB_SEMICONT;
		}
		else if (type == "semint") {
			vtype = GRB_SEMIINT;
		}
		numVars++;
		model->addVar(lowerbound, upperbound, obj, vtype, name);



	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void GurobiModel::changeNonzeros(int nonzeros, string name)
{
	try {
		model->set(GRB_IntAttr_NumNZs, nonzeros);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}


}

void GurobiModel::removeVar(string name)
{
	try {
		GRBVar var = model->getVarByName(name);
		model->remove(var);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void GurobiModel::addConstraint(double rightSide, string sense, string name, double lowerbound)
{

	try {
		char s;

		if (sense == "<=") {
			s = GRB_LESS_EQUAL;
		}
		else if (sense == "=") {
			s = GRB_EQUAL;
		}
		else {
			s = GRB_GREATER_EQUAL;
		}
		//The expr its made with 0, only after(setAllVarsConstraintCoeffs or setConstraintCoeffs) we build the expr of the constr
		model->addConstr(0, s, rightSide, name);
		numConstraints++;
		//model->update();

	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}



}


void GurobiModel::updateModel() {
	model->update();

}

void GurobiModel::removeConstraint(string name)
{
	try {
		GRBConstr constr = model->getConstrByName(name);
		numConstraints--;
		model->remove(constr);
		model->update();
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

int GurobiModel::getNumConstraints()
{
	//return model->get(GRB_IntAttr_NumConstrs);
	model->update();
	return numConstraints;
}

int GurobiModel::getNumVars()
{
	
	//The reason i did this is because we need to update the model before we call getNumVars, and maybe we haven't call it earlier.
	model->update();
	return model->get(GRB_IntAttr_NumVars);
}

/* In Gurobi, we don't have any function to add the vars after the constraints. 
So we use the constr added before to make a new one,
with the same attributes of the old. Then we delete the old and we have a new constraint with everything we need.

*/
void GurobiModel::setAllVarsConstraintCoeffs(const double *coeffs)
{
	//var to keep the old constr
	GRBConstr constr;
	expr.assign(numConstraints, GRBLinExpr());
	//We update the model here as cautiously act. Because of the "Lazy Update" System GRB uses, so we have assurance the constr and vars are in the the model.
	model->update();
	int k;
	try {
		GRBVar *vars = model->getVars();
		for (int i = 0; i < numConstraints; i++) {
			for (int j = 0; j < getNumVars(); j++) {
				k = i * getNumVars() + j; // The Array coeffs it's a linearized matrix, so we need this line to access the index correctly
				expr[i] += GRBLinExpr(vars[j], coeffs[k]);

			}
			constr = model->getConstr(i);
			model->addConstr(expr[i], constr.get(GRB_CharAttr_Sense), constr.get(GRB_DoubleAttr_RHS), constr.get(GRB_StringAttr_ConstrName));
			model->update();

		}
		//deleting the old constrs
		for (int i = 0; i < numConstraints; i++) {
			constr = model->getConstr(i);
			model->remove(constr);
		}
		model->update();


	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void GurobiModel::setConstraintCoeffs(vector <double> coeffs, string name)
{

	try {
		GRBConstr constr = model->getConstrByName(name);
		GRBVar *vars = model->getVars();
		GRBLinExpr	  expr = 0;
		for (int i; i < getNumVars(); i++) {
			expr += GRBLinExpr(vars[i], coeffs[i]); // build the expression of a costr per time
		}
		model->addConstr(expr, constr.get(GRB_CharAttr_Sense), constr.get(GRB_DoubleAttr_RHS), constr.get(GRB_StringAttr_ConstrName));
		model->update();
		// In Gurobi, we add a new contraint and we delete the old one
		model->remove(constr);
	}
	catch (GRBException  e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void GurobiModel::setConstraintCoeffs(const double * coeffs, int constrIndex)
{
	model->update();
	GRBConstr c = model->getConstr(constrIndex);
	for (int i = 0; i < numVars; i++) {
		GRBVar v = model->getVar(i);
		model->chgCoeff(c, v, coeffs[i]);
	}
}

void GurobiModel::setConstraintCoeffs(const double coeff, int indexConstr, int indexVar)
{

	GRBConstr c = model->getConstr(indexConstr);
	GRBVar v = model->getVar(indexVar);
	model->chgCoeff(c, v, coeff);
}

//Here we actually change the coeff, so almost everything said above isn't nedded
void GurobiModel::chgCoeff(string constrName, string varName, double coeff)
{
	model->update();
	try {
		GRBConstr c = model->getConstrByName(constrName);
		GRBVar v = model->getVarByName(varName);
		model->chgCoeff(c, v, coeff);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}



void GurobiModel::buildModel(string sense)
{
	model->update();
	try {
		int s;
		if (sense == "maximize") {
			s = GRB_MAXIMIZE;
		}
		else {
			s = GRB_MINIMIZE;
		}
		GRBVar *vars = model->getVars();
		GRBLinExpr expr = 0;
		for (int i = 0; i < getNumVars(); i++) {
			cout << vars[i].get(GRB_DoubleAttr_Obj) << endl;
			expr += GRBLinExpr(vars[i], vars[i].get(GRB_DoubleAttr_Obj)); // build the objective expression

		}
		model->setObjective(expr, s);

		model->optimize();
		cout << "Obj: " << model->get(GRB_DoubleAttr_ObjVal) << endl;







		const int numVars = model->get(GRB_IntAttr_NumVars);







	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

//----------- params
void GurobiModel::setParamTimeLimit()
{
	try {
		model->set(GRB_DoubleParam_TimeLimit, 100.0);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}

}

void GurobiModel::setParamTimeLimit(double timeLimit)
{
	try {
		model->set(GRB_DoubleParam_TimeLimit, timeLimit);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}

}

void GurobiModel::setParamTolerance()
{
	try {
		//model->set(GRB_DoubleParam_TimeLimit, timeLimit);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

void GurobiModel::setParamMIPStart(double *values)
{
	try {
		GRBVar *vars = model->getVars();
		for (int i = 0; i < getNumVars(); i++) {
			vars[i].set(GRB_DoubleAttr_Start, values[i]);
		}

	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}


void GurobiModel::setParamMIPStart(string filename)
{

	try {
		model->read(filename);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}



void GurobiModel::setParamRins()
{
	setParamRins(-1);
}

void GurobiModel::setParamRins(int attribute)
{
	try {
		model->set(GRB_IntParam_RINS, attribute);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}

}

void GurobiModel::setParamVarSel()
{
	setParamVarSel(-1);


}

void GurobiModel::setParamVarSel(int value)
{
	try {
		model->set(GRB_IntParam_VarBranch, value);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}


}

void GurobiModel::printVarsInSol()
{
	GRBVar *vars = model->getVars();
	for (int i = 0; i < getNumVars(); i++) {
		if (vars[i].get(GRB_DoubleAttr_X) != 0) {
			cout << vars[i].get(GRB_DoubleAttr_X) << endl;
			cout << vars[i].get(GRB_StringAttr_VarName) << endl;
		}
	}

}

vector<double> GurobiModel::getVarsInSol()
{
	GRBVar *vars = model->getVars();
	vector <double> values;
	for (int i = 0; i < getNumVars(); i++) {
		if (vars[i].get(GRB_DoubleAttr_X) != 0) {
			values.push_back(i);
		}
	}
	return values;
}

OptGurobi::OptGurobi()
{
}

OptGurobi::~OptGurobi()
{
}