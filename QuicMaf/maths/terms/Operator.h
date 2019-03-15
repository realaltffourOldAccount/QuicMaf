#ifndef OPERATOR_H
#define OPERATOR_H
#pragma once

using namespace std;
#include "Term.h"
#include "../defines.h"

class Operator : public Term {
public:
	Operator(CValue oper) : Term() { 
		mOperator = oper;
		mType = TermTypes::Op; 
	}

	string to_str() override {
		stringstream str;
		str << mOperator;
		return str.str();
	}

};
#endif // !OPERATOR_
