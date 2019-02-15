#ifndef VARIABLE_H
#define VARIABLE_H
#pragma once
#include "Term.h"
#include "../defines.h"

using namespace std;

class Variable : public Term {
public:
	Variable(NValue val = 0, CValue var = DEF_C, NValue pwr = 1) {
		mValue = val;
		mVariable = var;
		mPower = pwr;
		mType = TermTypes::Var;
	}

};
#endif // !VARIABLE_H
