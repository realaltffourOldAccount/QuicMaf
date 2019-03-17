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
	Bracket() : Term() {
		mType = TermTypes::Brack;
	}

	Bracket(vector<Term*> ts) : Term() {
		mType = TermTypes::Brack;
		mTerms = ts;
	}

	vector<Term*> mTerms;
	Term *mConstant = nullptr; // Nullptr == 1


	string to_str() override {
		string str;

		if (mConstant != nullptr)
			str.append(mConstant->to_str());

		str.append("(");

		for (int i = 0; i < mTerms.size(); i++)
			str.append(mTerms[i]->to_str());

		str.append(")");
		return str;
	}
};
#endif // !BRACKET_H
