#pragma once
#include "Opt.h"
#include <ilcplex/cplex.h>
#include <ilcplex/ilocplex.h>
#include <vector>
using namespace std;
class OptCplex : public Opt {
private:
public:
};

class CplexModel : public Model {
private:
	IloEnv			env;
	IloCplex		cplex;
	IloModel		model;
	int				numConstraints;
	int				numVars;
	int				nonzeros;
	IloNumVarArray  vars;
	IloObjective	objective;
	IloRangeArray	constr;
	IloConstraintArray constr2;
	vector <float>	coeffsObj;
public:
	CplexModel(const char *name, IloEnv env);
	~CplexModel();
	IloModel	getModel();
	void		imprimirInformacoes();
	void		addVar(double upperbound, double obj, string name, string type = "float", double lowerbound=0);
	float around(float var);
	void		changeNonzeros(int nonzeros, string name);
	void		removeVar(string name);
	void		removeVar(string name, bool deleteAll);
	void		addConstraint(double rightSide, string type, string name, double lowerbound = 0);
	void        addConstraint(double coeff, int rhsVarId, int lhsVarId, string type, string name, double lowerbound);
	void        addConstraint(int rhsVarId, int lhsVarId, string type, string name, double lowerbound);
	void		removeConstraint(string name);
	void		removeConstraint(string name, bool deleteAll);
	int			getNumConstraints();
	int			getNumVars();
	void		setAllVarsConstraintCoeffs(const double* Coeffs);
	void		setConstraintCoeffs(vector <double> coeffs, string name);
	void		setConstraintCoeffs(const double *coeffs, int constrIndex);
	void		setConstraintCoeffs(const double coeff, int indexConstr, int indexVar);
	void		chgCoeff(string constrName, string varName, float coeff);
	void		buildModel(string sense);
	void        buildModel(string sense, int varMaxDisp, int varMaxDist);
	void		setParamTimeLimit();
	void		setParamTimeLimit(double time);
	void		setParamTolerance();
	void        setParamMIPStart(double *values);
	void		setParamMIPStart(int value);
	void		setParamRins();
	void		setParamRins(int value);
	void		setParamVarSel();
	void		setParamVarSel(int value);
	void		printVarsInSol();
	vector <int> getVarsInSol();



	/**
	Adds a named MIP start with the value of all variables and a name.
	Unlike the method setStart, the method addMIPStart is not incremental. In other words, each call of addMIPStart creates a new MIP start.
	@param double *values  Array with the values of the vars
	@param string  name    name of the MIP Start
	@param int     MIPEnum This enumeration defines the possible levels of effort for CPLEX to expend when CPLEX evaluates a MIP start


	Level 0 (zero) MIPStartAuto: Automatic, let CPLEX decide.
	Level 1 (one) MIPStartCheckFeas: CPLEX checks the feasibility of the MIP start.
	Level 2 MIPStartSolveFixed: CPLEX solves the fixed problem specified by the MIP start.
	Level 3 MIPStartSolveMIP: CPLEX solves a subMIP.
	Level 4 MIPStartRepair: CPLEX attempts to repair the MIP start if it is infeasible, according to the parameter that sets the frequency to try to repair infeasible MIP start, CPXPARAM_MIP_Limits_RepairTries (that is, IloCplex::Param::MIP::Limits::RepairTries).
	Level 5 MIPStartNoCheck: CPLEX accepts the MIP start without any checks. The MIP start needs to be complete.
	@return none
	*/
	virtual void addStartMIP(double *values, string name, int MIPEnum);
};