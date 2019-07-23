#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;
//!Super Class of OptGurobi and OptCplex . 
/*!
Interface (Virtual Class).
*/

enum {
	SOLVER_CPLEX,
	SOLVER_GUROBI
};
class Opt
{
private:

public:


	Opt();
	~Opt();
};

class Param {
public:
	Param();
	~Param();
	bool fixedNumClusters;
	int numMaxIterations;
};





class Model {
private:
public:
	Model();
	~Model();

	//Model* createModel(const char *name, Environment* env, int Solver);
	/**
	add a Variable to Model
	@param  double lowerbound Variable lower bound (default = 0)
	@param  double upperbound Variable upper bound (default = Infinity)
	@param  string name	   Name of the Var (required)
	@param  string type	   Type of the Var, (default = float) float and int are available in both childs, bool (only in cplex), binary, semi integer and semi continuos (only in gurobi)
	@return none
	*/
	virtual void addVar(double upperbound, double obj, string name, string type, double lowerbound) = 0;
	/**
	change the number of nonzeros
	@param  int nonzeros Number of nonzeros variables in the model
	@param  string name  Name
	@return none
	*/
	virtual void changeNonzeros(int nonzeros, string name) = 0;
	/**
	Remove a variable from the model
	@param  string name Name of the Variable to be removed
	@return none
	*/
	virtual void removeVar(string name) = 0;
	/**
	Set all Variables Coefficients
	<br>Set the Coefficients of Variables in each Constraint. Array size should be the same as Constraints number x Vars number.<br>
	@param  const double *coeffs Array of coefficients, its a linearized matrix
	@return none
	*/
	virtual void setAllVarsConstraintCoeffs(const double *coeffs) = 0;
	/** Set the Coefficients of Variables in one Constraint
	@param  const double *coeffs Array of coefficients, including zeros
	@return none
	*/
	virtual void setConstraintCoeffs(vector <double> coeffs, string name) = 0;
	/**
	Set all the coefficients of a constraint,
	@param  string name constraint name
	@param  double coeff var coefficient
	@return none
	*/
	virtual void setConstraintCoeffs(const double *coeffs, int constrIndex) = 0;
	/**
	Set the coefficient of a variable,
	@param  string name constraint name
	@param  double coeff var coefficient
	@return none
	*/
	virtual void setConstraintCoeffs(const double coeff, int indexConstr, int indexVar) = 0;
	/**
	Set all the coefficients of a constraint,
	@param  int constrindex constraint index
	@param  double coeff      constraint coefficients
	@return none
	*/

	virtual void chgCoeff(string constrName, string varName, float coeff) = 0;
	/**
	add a Constraint to the Model, constraints are build using Ranges
	@param double lowerbound Constraint lower bound (default = 0)
	@param	double rightSide Constraint right side of (required)
	@param string sense     Constraint sense		(required)
	@param string name		 Constraint name		(required)
	@return none
	*/
	virtual void addConstraint(double rightSide, string sense, string name, double lowerbound) = 0;
	/**
	return the number of Constraints
	@param none
	@return int numConstraints, the number of constraints in model
	*/
	virtual void addConstraint(double coeff, int rhsVarId, int lhsVarId, string type, string name, double lowerbound) = 0;
	virtual int getNumConstraints() = 0;
	/**
	remove a constraint which name is given
	@param  string name
	@return none
	*/
	virtual void removeConstraint(string name) = 0;
	/**
	return the number of Variables
	@param none
	@return int numVars, the number of vars in model
	*/
	virtual int getNumVars() = 0;
	
	/**
	Build Model, setting the objective expression, and then Solving the model.
	@param double string sense, Objective Expression sense
	@return none
	*/
	virtual void buildModel(string sense) = 0;
	
	
	/**
	Limits the total time expended
	@param double timeLimit time in seconds
	@return none
	*/
	virtual void setParamTimeLimit(double timeLimit) = 0;
	/**
	@param double
	@return none
	*/

	virtual void setParamTolerance() = 0;
	/**
	Specifies that OPT should use advanced starting information
	<br><b>[Cplex]</b>
	<table>
	<tr> <td> Do not use advanced start information</td> <td> 0 </td> </tr>
	<tr> <td> Use an advanced basis supplied by the user, default</td> <td> 1 </td> </tr>
	<tr> <td> Crush an advanced basis or starting vector supplied by the user </td> <td> 2 </td></tr>
	</table>
	<b>[Cplex]</b>
	@param int value

	@return none
	*/
	virtual void setParamMIPStart(double *values) = 0;
	/**
	Decides how often to apply the (RINS) heuristic.
	<br><b>[Cplex]</b>
	<table>
	<tr> <td> OFF </td> <td> -1 </td> </tr>
	<tr> <td> Automatic, Default </td> <td> 0 </td> </tr>
	<tr> <td> Any positive integer Frequency to apply RINS </td> <td> Frequency </td></tr>
	</table>
	<b>[Cplex]</b>
	<br><b>[Gurobi]</b>
	<table>
	<tr> <td> Automatic (-1) </td> <td> -1 </td> </tr>
	<tr> <td> OFF </td> <td> 0 </td> </tr>
	<tr> <td> A positive value n applies RINS at every n-th node of the MIP search tree </td> <td> Frequency </td></tr>
	</table>
	<b>[Gurobi]</b>
	@param  int value  Frequency to apply RINS

	@return none
	*/
	virtual void setParamRins(int value) = 0;
	/**
	Control selection of the next variable
	<br><b>[Cplex]</b>
	<table>
	<tr> <td>   Branch on variable with minimum infeasibility  </td> <td> -1 </td> </tr>
	<tr> <td>   Automatic, default </td> <td> 0 </td> </tr>
	<tr> <td>   Pseudo Shadow Price Branching </td> <td> 1 </td></tr>
	<tr> <td>   Maximum Infeasibility Branching  </td> <td> 2 </td></tr>
	<tr> <td>   Strong Branching  </td> <td> 3 </td></tr>
	</table>
	<b>[Cplex]</b>
	<br><b>[Gurobi]</b>
	<table>
	<tr> <td>   Automatic choice </td> <td> -1 </td> </tr>
	<tr> <td>   Pseudo Reduced Cost Branching</td> <td> 0 </td> </tr>
	<tr> <td>   Branch on variable with maximum infeasibility </td> <td> 1 </td></tr>
	<tr> <td>   Branch based on pseudo costs  </td> <td> 2 </td></tr>
	<tr> <td>   Strong branching  </td> <td> 3 </td></tr>
	<tr> <td>   Branch based on pseudo reduced costs </td> <td> 4 </td></tr>
	</table>
	<b>[Gurobi]</b>
	@param  int value  parameter influences which variable is chosen for branching at that node
	@return none
	*/
	virtual void setParamVarSel(int value) = 0;

	
	virtual void printVarsInSol() = 0;

	


	virtual vector<int> getVarsInSol() = 0;


};