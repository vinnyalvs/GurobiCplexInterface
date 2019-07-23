#pragma once
#include <iostream>
#include <vector>
#include "Opt.h"

class OptGurobi : public Opt {
private:

public:
	OptGurobi();
	~OptGurobi();
};

class GurobiModel : public Model {
private:
	//Number of Contraints and Variables of the problem 
	int				numConstraints;
	int				numVars;
	//Structures to kepp data, vars are associated directly to the model, that's why they are not here
	GRBModel		*model;
	GRBLinExpr		objective;
	vector <GRBLinExpr> expr;
	vector <GRBConstr> constraints;
public:
	GurobiModel(string name, GRBEnv env);
	~GurobiModel();
	GRBModel getModel();
	void	addVar(double upperbound, double obj, string name, string type="float", double lowerbound = 0);
	void	changeNonzeros(int nonzeros, string name);
	void	removeVar(string name);
	void	addConstraint(double rightSide, string sense, string name, double lowerbound=0);
	void    updateModel();
	void	removeConstraint(string name);
	int		getNumConstraints();
	int		getNumVars();
	void	setAllVarsConstraintCoeffs(const double *coeffs);
	void	setConstraintCoeffs(vector <double> coeffs, string name);
	void	setConstraintCoeffs(const double *coeffs, int constrIndex);
	void    setConstraintCoeffs(const double coeff, int indexConstr, int indexVar);
	void	chgCoeff(string constrName, string varName, double coeff);
	void	buildModel(string sense);
	void	setParamTimeLimit();
	void	setParamTimeLimit(double timeLimit);
	void	setParamTolerance();
	void	setParamMIPStart(double *values);
	void	setParamMIPStart(string filename);
	void	setParamRins();
	void	setParamRins(int attribute);
	void	setParamVarSel();
	void    setParamVarSel(int value);
	void	printVarsInSol();
	vector<double> getVarsInSol();
	
	bool    modelNeedUpdate;
};
