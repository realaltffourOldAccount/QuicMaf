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
	}

	Bracket(vector<Term*> ts) {
		Term();
		mType = TermTypes::Brack;
		mTerms = ts;
	}

	vector<Term*> mTerms;

	void setConstant(Term* constant) {
		mConstant = constant;
	}
	
	Term* GetConstant() const { return mConstant; }

private:
	Term *mConstant;
};
#endif // !BRACKET_H
