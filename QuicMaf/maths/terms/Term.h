#ifndef TERM_H
#define TERM_H

#pragma once
#include "../defines.h"
using namespace std;

class Term {
public:
	Term(NValue val = DEF_N, CValue var = DEF_C, CValue oper = DEF_C, NValue pwr = 1) { 
		mValue = val; 
		mVariable = var;
		mOperator = oper;
		mPower = pwr;
	}

	virtual string to_str() {
		return "TERM_UNKNOWN"; 
	};

	NValue mValue = DEF_N;
	CValue mVariable = DEF_C;
	NValue mPower = 1;
	CValue mOperator = DEF_C;
	bool mSquareRoot = false;
	int mType = -1;
};
#endif // !TERM_H
