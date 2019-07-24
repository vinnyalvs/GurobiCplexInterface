#pragma once
#include "Opt.h"
#include "OptCplex.h"
#include "OptGurobi.h"

class Environment {
private:
	int solver;
	GRBEnv *grbEnv;
	IloEnv *cplexEnv;
	GurobiModel *mdlGRB;
	CplexModel	*mdlCplex;
public:
	Environment(int solver);
	GRBEnv *getGRBEnv();
	IloEnv *getCplexEnv();
	GurobiModel *getMdlGRB();
	CplexModel *getMdlCplex();
	Model *getModel();
	void setCplexEnv(IloEnv *CplexEnv);
	void setGrbEnv(GRBEnv *grbEnv);
	void setSolver(int solver);
	int getSolver();

};