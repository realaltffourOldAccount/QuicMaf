#ifndef SOLVER_H
#define SOLVER_H
#pragma once
#include "../Equations.h"
#include "../defines.h"
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
	void Parse(string equation);
	Equation* Solve();

public: // PRIVATE MEMBERS TODO: MARK PRIVATE IN FINAL BUILD
	Equation* mEquation = nullptr;

private: // Mathematics Side Class Todo: Mark Private in final build
	vector<Term*> FlipTermSigns(vector<Term*> terms);

	Equation* ApplyDiversionCalcs(Equation* equ);

private: // Program Side Calcs TODO: MARK PRIVATE IN FINAL BUILD
	bool CheckValidOps(Equation* equ);

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
