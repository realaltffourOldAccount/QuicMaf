#ifndef BRACKET_H
#define BRACKET_H
#pragma once

#include "Term.h"
#include "Variable.h"
#include "../defines.h"
#include "Constant.h"
using namespace std;

class Bracket : public Term {
public:
	Bracket() {
		Term();
		mType = TermTypes::Brack;
		mConstant = new Constant(1);
	}

	vector<Term*> mTerms;

	void setConstant(Term* constant) {
		mConstant = constant;
	}

private:
	Term *mConstant;
};
#endif // !BRACKET_H
