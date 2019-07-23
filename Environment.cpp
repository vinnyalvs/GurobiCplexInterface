#include "Environment.h"


Environment::Environment(int solver)
{
	this->solver = solver;
	if (solver == 1) {
		this->cplexEnv = new IloEnv();
		this->mdlCplex = new CplexModel("CplexModel",*cplexEnv);
		this->mdlGRB = NULL;
		this->grbEnv = NULL;
	}
	else if (solver == -1) {
		this->grbEnv = new GRBEnv();
		this->mdlGRB = new GurobiModel("GRBModel", *grbEnv);
		this->cplexEnv = NULL;
		this->mdlCplex = NULL;
	}

	cout << "alo" << endl;
}

GRBEnv * Environment::getGRBEnv()
{
	return this->grbEnv;
}

IloEnv * Environment::getCplexEnv()
{
	return this->cplexEnv;
}

GurobiModel * Environment::getMdlGRB()
{
	return this->mdlGRB;
}

CplexModel * Environment::getMdlCplex()
{
	return this->mdlCplex;
}

void Environment::setCplexEnv(IloEnv  *cplexEnv)
{
	this->cplexEnv = cplexEnv;
}


void Environment::setGrbEnv(GRBEnv * grbEnv)
{
	this->grbEnv = grbEnv;
}

void Environment::setSolver(int solver)
{
	this->solver = solver;
}

int Environment::getSolver()
{
	return solver;
}
