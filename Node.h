#pragma once

#include "NData.h"

class __declspec(dllexport) Node
{
public:
	NData	data;
	Node*	up;
	Node*	horizontal;
	Node*	down;
	Node*	sibling;
	int		level;

	Node(double price)
		: up(0), horizontal(0), down(0), sibling(0) {
		data.sharePrice = price;
	}
};

