#pragma once

#define NDATA_H

class __declspec(dllexport) NData
{
public:
	double	sharePrice;
	double	optionPrice;

	NData() : sharePrice(0.0), optionPrice(0.0) {};
};

