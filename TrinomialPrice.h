#pragma once
#include "Option.h"
#include "Lattice.h"

class __declspec(dllexport) TrinomialPrice
{
public:
	Option*		option;
	int			steps;
	Lattice*	lattice;

	TrinomialPrice(Option* pOpt, int nSteps);

	void buildLattice(Node* node);
	void backwardEval(Node*);

private:
	double			dt;		// delta time
	double			u;		// up factor
	double			d;		// down factor = 1/u
	double			pu;		// up probability
	double			ph;		// horizontal probability
	double			pd;		// down probability = 1 - pu
	double			discount;

	const double lambda = 1.2;
};