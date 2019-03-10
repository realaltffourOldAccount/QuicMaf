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

#include "../../vendor/lexertk.hpp"

class Solver
{
public:
	void Parse(string equation);
	Term* Solve();

public: // PRIVATE MEMBERS TODO: MARK PRIVATE IN FINAL BUILD
	Equation* equation = nullptr;
public: // Program Side Calcs TODO: MARK PRIVATE IN FINAL BUILD
	bool CheckValidOps(Equation* equ);

	Equation* RemNonEssentialOp(Equation* equ);
	bool CheckOpRemovalValidity(Operator* op);
	bool CheckOpT2RemovalNeed(Operator* op);
	Equation* BreakUselessBracks(Equation* equ);
	Equation* RemoveWeightOfWings(Equation* equ);
	Equation* OrderTerms(Equation* equ);

	Equation* CheckDiversionCalcs(Equation* equ);
	Equation* DoDiversionCalcs(Equation* equ);
	
	Term* MergeOpTerm(Operator* op, Term* t1, Term* t2);

public: // Math Side Calcs TODO: MARK PRIVATE IN FINAL BUIlD
	Equation* Evaluate(Equation* equ);
};

#endif // !1
