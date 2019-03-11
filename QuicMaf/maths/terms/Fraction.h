#ifndef FRACTION_H
#define FRACTION_H
#pragma once
#include "../defines.h"
#include "Term.h"
using namespace std;

class Fraction: public Term {
public:
	Fraction(vector<Term*> nomin = {}, vector<Term*> domin = {}) {
		Term();
		mNomin = nomin;
		mDomin = domin;
		mType = TermTypes::Frac;
	}

public:	 // TODO: try to change to private
	vector<Term*> mNomin;
	vector<Term*> mDomin;
};
#endif // !CONSTANT_H
