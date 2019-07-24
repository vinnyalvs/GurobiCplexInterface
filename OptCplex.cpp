#include "OptCplex.h"

/**
CS-11 Asn 2
OptCplex.cpp
Purpose: Implements Opt for Cplex optimizer

@author Vinicius Alves
@version 1.1 12/12/16
*/


CplexModel::CplexModel(const char *name, IloEnv env)
{
	try {
		this->env = env;
		model = IloModel(this->env, name);
		cplex = IloCplex(model);
		vars = IloNumVarArray(env);
		constr = IloRangeArray(env);
		objective = IloObjective(env);
		numVars = 0;
		numConstraints = 0;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}


CplexModel::~CplexModel()
{
}

IloModel CplexModel::getModel()
{
	return model;
}

void CplexModel::imprimirInformacoes()
{
}

void CplexModel::addVar(double upperbound, double obj, string name, string type, double lowerbound) {
	IloNumVar::Type t;
	if (type == "float" || type == "double") {
		t = ILOFLOAT;
	}
	else if (type == "int") {
		t = ILOINT;
	}
	else if (type == "boolean") {
		t = ILOBOOL;
	}
	try {
		vars.add(IloNumVar(env, lowerbound, upperbound, t, name.c_str()));
		coeffsObj.push_back(obj);
		numVars++;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}


void CplexModel::addConstraint(double rightSide, string type, string name, double lowerbound) {
	try {



		constr.add(IloRange(env, lowerbound, rightSide, name.c_str()));

		numConstraints++;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}

void CplexModel::addConstraint(double coeff, int rhsVarId, int lhsVarId, string type, string name, double lowerbound) {
	try {
		model.add(coeff * vars[lhsVarId] - vars[rhsVarId] <= 0);
		numConstraints++;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}



void CplexModel::removeConstraint(string name)
{
	removeConstraint(name, false);
}
/**
Remove a constraint from the model
@param  string name Name of the constraint to be removed
@param  bool   deleteAll, this var indicates if all appearances of the name in constraint array should be removed, [true] delete all appearances [false] just the first
@return none
*/
void CplexModel::removeConstraint(string name, bool deleteAll)
{
	for (int i = 0; i < numConstraints; i++) {
		if (constr[i].getName() == name.c_str()) {
			constr[i].end();
			numConstraints--;
			if (!deleteAll)
				break;
		}
	}
}
void CplexModel::removeVar(string name)
{
	removeVar(name, false);
}

/**
Remove a variable from the model
@param  string name Name of the variable to be removed
@param  bool   deleteAll, this indicates if all appearances of the name in variable array should be removed, [true] delete all appearances [false] just the first
@return none
*/

void CplexModel::removeVar(string name, bool deleteAll)
{
	for (int i = 0; i < numVars; i++) {
		if (vars[i].getName() == name.c_str()) {
			vars[i].end();
			numVars--;
			if (!deleteAll)
				break;
		}
	}
}


int CplexModel::getNumConstraints()
{
	return numConstraints;
}

int CplexModel::getNumVars()
{
	return numVars;
}



void CplexModel::setAllVarsConstraintCoeffs(const double *coeffs)
{

	int k = 0;
	try {

		for (int i = 0; i < numConstraints; i++) {
			for (int j = 0; j < numVars; j++) {
				k = i * numVars + j; // The Array coeffs it's a linearized matrix, so we need this line to access the index correctly
				constr[i].setLinearCoef(vars[j], coeffs[k]);

			}

		}
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}

void CplexModel::setConstraintCoeffs(vector <double> coeffs, string name)
{

	for (int i = 0; i < numConstraints; i++) {
		if (strcmp(constr[i].getName(), name.c_str())) {
			for (int j = 0; j<numVars; j++)
				constr[i].setLinearCoef(vars[j], coeffs[j]);
			break;
		}
	}

}

void CplexModel::setConstraintCoeffs(const double * coeffs, int indexConstr)
{

	for (int i = 0; i < numVars; i++) {
		constr[indexConstr].setLinearCoef(vars[i], coeffs[i]);
	}


}

void CplexModel::setConstraintCoeffs(const double coeff, int indexConstr, int indexVar)
{


	constr[indexConstr].setLinearCoef(vars[indexVar], coeff);




}

void CplexModel::chgCoeff(string constrName, string varName, float coeff)
{
	int varIndex;
	//Change the coeff in a constraint.First, interate through each index of variable array and find the index we are looking for.
	//Then interate trough a second loop to find the constraint and change the coeff
	for (int i = 0; i < getNumVars(); i++) {
		if (vars[i].getName() == constrName.c_str()) {
			varIndex = i;
			break;
		}
	}
	try {
		for (int i = 0; i < getNumConstraints(); i++) {
			if (constr[i].getName() == constrName.c_str()) {
				constr[i].setLinearCoef(vars[varIndex], coeff);
				break;
			}
		}
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}

}

void CplexModel::buildModel(string sense)
{
	//

	model.add(vars);
	model.add(constr);
	model.add(objective);

	if (sense == "maximize")
		objective.setSense(IloObjective::Maximize);
	else
		objective.setSense(IloObjective::Minimize);


	//cout << std::fixed << endl;
	for (int i = 0; i<numVars; i++) {
		//cout << coeffsObj[i] << endl;
		objective.setLinearCoef(vars[i], coeffsObj[i]);
	}
	try {
        cplex.exportModel("arquivo.lp");
		cout << "----------------------------------------" << endl;
		cplex.solve();

		cout << endl;
		cout << "Solution status: " << cplex.getStatus() << endl;
		cout << "Value: " << cplex.getObjValue() << endl;
		cout << endl;
		cout << "----------------------------------------" << endl;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}

}



void CplexModel::buildModel(string sense, int varMaxDisp, int varMaxDist)
{
	//

	IloObjective *obj;
	//obj.setExpr(vars[varMaxDisp] + vars[varMaxDist]);
	//	obj = new IloObjective(env, vars[varMaxDisp] * vars[varMaxDist], IloObjective::Minimize);
	//obj.setSense(IloObjective::Minimize);


	obj = new IloObjective(env, vars[varMaxDisp], IloObjective::Minimize);

	model.add(vars);
	model.add(constr);
	//cplex.setParam(IloCplex::Param::SolutionType, 2);
	//	model.add(objective);
	model.add(*obj);


	try {


		cout << "----------------------------------------" << endl;
		cplex.setParam(IloCplex::EpRHS, 1e-1);
		cplex.solve();

		cplex.exportModel("arquivo.lp");

		cout << endl;
		cout << "Solution status: " << cplex.getStatus() << endl;
		cout << "Value: " << cplex.getObjValue() << endl;
		cout << endl;
		cout << "----------------------------------------" << endl;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}

}

void CplexModel::setParamTimeLimit()
{
	setParamTimeLimit(100);
}

void CplexModel::setParamTimeLimit(double time)
{
	try {
		cplex.setParam(IloCplex::NumParam::TiLim, time);
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
	cplex.setParam(IloCplex::NumParam::TiLim, time);
}

void CplexModel::setParamTolerance()
{

}

void CplexModel::setParamMIPStart(double * values)
{
	addStartMIP(values, " ", 0);
}

void CplexModel::setParamMIPStart(int value)
{
	cplex.setParam(IloCplex::Param::Advance, value);
}

void CplexModel::setParamRins()
{
	setParamRins(0);
}

void CplexModel::setParamRins(int value)
{
	try {
		cplex.setParam(IloCplex::Param::MIP::Strategy::RINSHeur, value);
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}

}



void CplexModel::setParamVarSel()
{
	setParamVarSel(0);
}

void CplexModel::setParamVarSel(int value)
{
	try {
		cplex.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, value);
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
}

void CplexModel::printVarsInSol()
{
	for (int i = 0; i < vars.getSize(); i++) {
		if (cplex.getValue(vars[i]) != 0) {
			cout << vars[i].getName() << endl;
			cout << cplex.getValue(vars[i]) << endl;
		}
	}

}

vector<int> CplexModel::getVarsInSol()
{
	vector <int> values;
	for (int i = 0; i < vars.getSize(); i++) {

		if (cplex.getValue(vars[i]) != 0) {

			values.push_back(i);
			//cout << i << " " << cplex.getValue(vars[i]) << endl;

		}


	}
	return values;

}





void CplexModel::addStartMIP(double *values, string name, int MIPEnum)
{
	IloCplex::MIPStartEffort effort;
	switch (MIPEnum) {
	case 0:
		effort = IloCplex::MIPStartAuto;
		break;
	case 1:
		effort = IloCplex::MIPStartCheckFeas;
		break;
	case 2:
		effort = IloCplex::MIPStartSolveFixed;
		break;
	case 3:
		effort = IloCplex::MIPStartSolveMIP;
		break;
	case 4:
		effort = IloCplex::MIPStartRepair;
		break;
	case 5:
		effort = IloCplex::MIPStartNoCheck;
		break;
	}
	IloNumArray startVal(env);
	for (int i = 0; i <= numVars; i++) {
		startVal.add(values[i]);
	}
	cplex.addMIPStart(vars, startVal, effort, name.c_str());
}


void CplexModel::changeNonzeros(int nonzeros, string name)
{

}


