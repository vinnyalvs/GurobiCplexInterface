
# Gurobi Cplex Interface

This project allows the use of Gurobi and Cplex solvers from the same API, which allows you to switch between solvers easily.
Interface created for C ++ Language.

Documentation can be found at: https://vinnyalvs.github.io/GurobiCplexInterface/

## Getting Started
The project is only source and headers files so you can just download it and use it with only two lines:

#include "GurobiCplexInterface/Opt.h"  
#include "GurobiCplexInterface/Enviroment.h"


### Prerequisites:


 - Cplex version 12.0 or higher
 - Gurobi version 7.0 or higher
 - You can get free academic versions of both.
 
#### Recommended:
 - C++ IDE
	 - Visual Studio  (the "big" one, not VSCode)	
	 - Codeblocks

CLion is not recommended because is quite hard to configure the CMake file to use the library of Cplex and Gurobi, but feel free to prove me wrong! 

### Installing
As said above, you need to install the Cplex and Gurobi solvers and set them up with a IDE so you can use the interface.

You can find a tutorial to install Cplex here: https://www.ibm.com/support/knowledgecenter/en/SSSA5P_12.9.0/ilog.odms.studio.help/Optimization_Studio/topics/COS_installing.html

You can find a tutorial to install Gurobi here: 
https://www.gurobi.com/documentation/6.5/quickstart_mac/software_installation_guid.html

A Tutorial to configure Cplex in Codeblocks:
https://www-01.ibm.com/support/docview.wss?uid=swg21449771

A Tutorial to configure Cplex in Visual Studio:
[https://www.leandro-coelho.com/how-to-configure-ms-visual-studio-to-use-ibm-cplex-concert/](https://www.leandro-coelho.com/how-to-configure-ms-visual-studio-to-use-ibm-cplex-concert/)

A Tutorial to configure Gurobi in Visual Studio:
[https://support.gurobi.com/hc/en-us/articles/360013194392-How-do-I-configure-a-new-Gurobi-C-project-with-Microsoft-Visual-Studio-2017-](https://support.gurobi.com/hc/en-us/articles/360013194392-How-do-I-configure-a-new-Gurobi-C-project-with-Microsoft-Visual-Studio-2017-)


After that, you can just download the interface and include it!!


#### A brief example

//TODO explain things

	Modelo, slide três Uchoa.
	
	1,5x + 4,0x ≤ 24 
	3,0x + 1,5x ≤ 21 
	1,0x + 1,0x ≤ 8


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
	Model *model = NULL;
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



## Built With

* [Doxywizard]()
* [Visual Studio Community]() 
* [Cplex]()
* [Gurobi]()


## Authors
* Vinicius Alberto Alves (https://github.com/vinnyalvs)
* Prof. Dsc Lorenza Leão oliveira Moreno

## Acknowledgments

The authors thank the active and former members of GETComp (ufjf.br/getcomp) and the teachers and staff of Dept. of Computer Science at the Federal University of Juiz de Fora.
