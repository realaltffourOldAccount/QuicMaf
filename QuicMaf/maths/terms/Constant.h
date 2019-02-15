#ifndef CONSTANT_H
#define CONSTANT_H
#pragma once
#include "../defines.h"
#include "Term.h"
using namespace std;

class Constant : public Term {
public:
	Constant(NValue val = 0, NValue pwr = 1) {
		Term();
		mValue = val;
		mPower = pwr;
		mType = TermTypes::Const;
	}
};
#endif // !CONSTANT_H
