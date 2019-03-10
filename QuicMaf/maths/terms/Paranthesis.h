#ifndef PARANTHESIS_H
#define PARANTHESIS_H
#pragma once

using namespace std;
#include "Term.h"
#include "../defines.h"  

enum ParanTypes {
	NORM_PARAN = 1,
	BRAK_PARAN = 2,
	CURL_PARAN = 3
};

class Paranthesis : Term {
	Paranthesis();
	Paranthesis(bool isopening, int type);

	bool isOpening() { return __isopening; }
	int getType() { return __type; }

	void setOpening(bool is) { __isopening = is; }
	void setType(char type) { __type = type; }

private:
	bool __isopening = true;
	int __type = 0;
};


#endif // !PARANTHESIS_H
