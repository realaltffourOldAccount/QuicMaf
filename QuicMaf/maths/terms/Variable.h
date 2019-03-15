#ifndef VARIABLE_H
#define VARIABLE_H
#pragma once
#include "Term.h"
#include "../defines.h"

using namespace std;

class Variable : public Term {
public:
	Variable(NValue val = 1, CValue var = DEF_C, NValue pwr = 1) : Term() {
		mValue = val;
		mVariable = var;
		mPower = pwr;
		mType = TermTypes::Var;
	}

	string to_str() override {
		stringstream str;
		if (mValue != 1)
			str << setprecision(0) << mValue;
		str << (char)mVariable;
		if (mPower > 1) {
			str << "^";
			str << setprecision(0) << mPower;
		}
		return str.str();
	}

};
#endif // !VARIABLE_H
