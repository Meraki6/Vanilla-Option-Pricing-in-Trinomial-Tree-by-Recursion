#pragma once

#include "Node.h"

class __declspec(dllexport) Lattice
{
public:
	Node*	root;

	Lattice() : root(0), nLevel(0) {};
	void setLevel(int n) { nLevel = n; }
	int getLevel() { return nLevel; }

private:
	int		nLevel;

};

