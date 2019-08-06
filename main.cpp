#include "Opt.h"
#include "Environment.h"

int main() {

	const int cols = 2;
	const int rows = 3;
	double DATA[rows][cols] = { { 1.5,4},{3,1.5 }, {1.0, 1.0} }; // Example of DATA, which contains the coeficients of the variables

	//Allocating dynamic matrix
	double **coeffs = new double*[rows];
	for (int i = 0; i < rows; ++i) {
		coeffs[i] = new double[cols];
		for (int j = 0; j < cols; j++)
			coeffs[i][j] = DATA[i][j];
	}
	

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

	//deleting coeffs
	for (int i = 0; i < rows; ++i)
		delete[] coeffs[i];
	delete[] coeffs; 

	return 0; 
}


