#include "Opt.h"
#include "Environment.h"



int main() {
	const double coeffs[6] = { 1.5,4.0,3,1.5,1.0,1.0 }; //Problem Coeffs
	int solver = -1; // 1 for Cplex -1 for Gurobi
	Model *model= NULL;
	Environment *env = new Environment(solver); 
	model = env->getModel();
	model->addConstraint(24, "<=", "ConstraintOne",0);
	model->addConstraint(21, "<=", "ConstraintTwo",0);
	model->addConstraint(8, "<=", "ConstraintThree",0);
	model->addVar(10, 4, "x1", "double",0);
	model->addVar(10, 6, "x2", "double",0);
	model->setAllVarsConstraintCoeffs(coeffs);
	model->buildModel("maximize");
	return 0;
}