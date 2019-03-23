#ifndef SOLVER_H
#define SOLVER_H
#pragma once
#include "../defines.h"
#include "../Equations.h"
#include "../terms/Brackets.h"
#include "../terms/Constant.h"
#include "../terms/Equal.h"
#include "../terms/Operator.h"
#include "../terms/Term.h"
#include "../terms/Variable.h"
#include "../terms/Paranthesis.h"
#include "../tokenizer.h"
#include "../terms/Bundle.h"

#include "../../vendor/lexertk.hpp"

#include "term_rewriter/QMEvalHelper.h"
#include "term_rewriter/QMEvaluator.h"

class Solver {
public:
	Solver(LOG* _log = nullptr) {
		log = _log;
	}
	void Parse(string equation);
	Equation* Solve();

	bool CheckEqu(string equation);

	bool CheckValidOps(Equation* equ);
	bool CheckValidOps(vector<Term*> terms);
	vector<Term*> RemoveBrackets(vector<Term*> terms) {
		vector<Term*> res;
		for (int i = 0; i < terms.size(); i++) {
			auto term = terms[i];
			if (terms[i]->mType == TermTypes::Brack) {
				auto _terms = QMEvalHelper::BreakBracket(static_cast<Bracket*>(term));
				if (!CheckValidOps(_terms)) {
					return {};
				}
				_terms = RemoveBrackets(_terms);
				for (auto item : _terms) res.push_back(item);
			}
			else
				res.push_back(term);
		}
		return res;
	}
public: // PRIVATE MEMBERS TODO: MARK PRIVATE IN FINAL BUILD
	Equation* mEquation = nullptr;
	bool mIncorrect = false;
	LOG* log = nullptr;

private: // Mathematics Side Class Todo: Mark Private in final build
	vector<Term*> FlipTermSigns(vector<Term*> terms);

	Equation* ApplyDiversionCalcs(Equation* equ);

public: // Program Side Calcs TODO: MARK PRIVATE IN FINAL BUILD

	bool IsVarAval(vector<Term*> terms);

	vector<Term*> RemoveBundle(vector<Term*> res) {
		vector<Term*> final_res;

		for (int i = 0; i < res.size(); i++) {
			if (res[i]->mType == TermTypes::Bund) {
				auto result = BreakBundle(static_cast<Bundle*>(res[i]));
				for (int j = 0; j < result.size(); j++)
					final_res.push_back(result[j]);
			}
			else
				final_res.push_back(res[i]);
		}
		return final_res;
	}

	Equation* PutVarsInOneSide(Equation* equ);
	Equation* PutConstsInOneSide(Equation* equ);

	vector<Term*> RemNonEssientalOps(vector<Term*> terms);
	vector<Term*> AddNonEssientalOps(vector<Term*> terms);

	Bundle* BuildBundle(vector<Term*> term);
	vector<Term*> BreakBundle(Bundle* bundle);

	vector<Term*> BundleTermsDuo(vector<Term*> terms);
	vector<Term*> BundleTermsTri(vector<Term*> terms);

	bool IsEqualBundle(Bundle* b1, Bundle* b2);

	vector<Term*> OrderTermsByValue(vector<Term*> terms);
};

#endif
