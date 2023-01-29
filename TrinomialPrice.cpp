#include "TrinomialPrice.h"
#include <math.h>

// Set up the Trinomial Pricing Parameters
TrinomialPrice::TrinomialPrice(Option* pOpt, int nSteps)
	: option(pOpt), steps(nSteps)

	// TO DO:
{
	/* set parameters */
	dt = pOpt->maturity / nSteps;
	u = exp(lambda * pOpt->vol * sqrt(dt));
	d = 1 / u;
	double M = exp(pOpt->rate * dt);
	double V = pow(M, 2) * (exp(pow(pOpt->vol, 2) * dt) - 1);
	pu = ((V + pow(M, 2) - M) * u - (M - 1)) / ((u - 1) * (pow(u, 2) - 1));
	pd = (pow(u, 2) * (V + pow(M, 2) - M) - pow(u, 3) * (M - 1)) / ((u - 1) * (pow(u, 2) - 1));
	ph = 1 - pu - pd;
	discount = exp(-pOpt->rate * dt);
};

// Build the Trinomial Lattice recursively
void
TrinomialPrice::buildLattice(Node* t)
// TO DO:

{
	if (t == NULL) { // initialize lattice and build root
		Node* root = new Node(option->spotPrice);
		Lattice* tree = new Lattice;
		lattice = tree;
		lattice->root = root;
		lattice->setLevel(steps);
		buildLattice(lattice->root);
	}
	else { // sibling as upper node
		if (t->sibling) { // if t->sibling is not null, then t->up and t->horizontal can be connected to horizontal node and down node of t->sibling without additional build
			Node* down = new Node(t->data.sharePrice * d);
			down->level = t->level + 1;
			/* connect up and horizontal with nodes of sibling */
			t->up = t->sibling->horizontal;
			t->horizontal = t->sibling->down;
			t->down = down; // connect down with created node
			t->down->sibling = t->horizontal;
			if (down->level < lattice->getLevel()) // build down if level is within lattice step
				buildLattice(t->down);
		}
		else { // if t does not have sibling, no children nodes have been built yet, need to build all three nodes, up, horizontal and down
			Node* up = new Node(t->data.sharePrice * u), * horizontal = new Node(t->data.sharePrice), * down = new Node(t->data.sharePrice * d);
			up->level = horizontal->level = down->level = t->level + 1;
			/* connect nodes with t */
			t->up = up;
			t->horizontal = horizontal;
			t->down = down;
			/* assign sibling to each nodes */
			t->horizontal->sibling = up;
			t->down->sibling = horizontal;
			if (up->level < lattice->getLevel()) { // build children nodes if level is within lattice step
				buildLattice(t->up);
				buildLattice(t->horizontal);
				buildLattice(t->down);
			}
		}
	}
}

// Calculate the option value using recursion
void
TrinomialPrice::backwardEval(Node* t)
// TO DO:
{
	if (!(t->data.optionPrice) && (t->level == lattice->getLevel())) { // compute option payoff at maturity
		t->data.optionPrice = MAX(floor(pow(10, 4) * OPTION_MULTIPLIER(option->c_p) * (t->data.sharePrice - option->strikePrice)) / pow(10, 4), 0);
		return;
	}
	if (!(t->sibling)) { // if t->sibling is null, nodes have not been built yet and thus option prices for up and horizontal have not been calculated, need to compute
		backwardEval(t->up);
		backwardEval(t->horizontal);
	}
	backwardEval(t->down); // always need to compute option price of down nodes
	/* option->e_a is 0 for European and 1 for American, first term is exercise payoff, while second term is discounted payoff */
	t->data.optionPrice = MAX(option->e_a * OPTION_MULTIPLIER(option->c_p) * (t->data.sharePrice - option->strikePrice),
		discount * (pu * t->up->data.optionPrice + ph * t->horizontal->data.optionPrice + pd * t->down->data.optionPrice));
	return;
}