#ifndef BUNDLE_H
#pragma once
#include "../defines.h"
#include "Term.h"
using namespace std;

// An Abstract term for representing bundles of terms
// Ex: 2x/4x

class Bundle : public Term {
public:
	Bundle() : Term() { mType = TermTypes::Bund; }
	Bundle(vector<Term*> terms) : Term() { mType = TermTypes::Bund; mTerms = terms; }

	string to_str() override {
		string str;
		for (int i = 0; i < mTerms.size(); i++)
			str.append(mTerms[i]->to_str());
		return str;
	}

	vector<Term*> mTerms;
};

#endif // !BUNDLE_H
