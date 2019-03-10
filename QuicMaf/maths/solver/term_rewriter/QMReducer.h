#ifndef QMREDUCER_H
#define QMREDUCER_H
#pragma once

#include "../../Equations.h"
#include "../../defines.h"
#include "../../terms/Brackets.h"
#include "../../terms/Constant.h"
#include "../../terms/Equal.h"
#include "../../terms/Operator.h"
#include "../../terms/Term.h"
#include "../../terms/Variable.h"
#include "../../terms/Paranthesis.h"
#include "../../tokenizer.h"

#include "../../../vendor/lexertk.hpp"

#include "QMRule.h"
#include "QMRuleSet.h"

// WARINING: Only one variable is allowed or errors will happen!

class QMReducer {
public:
	void setRuleSet(QMRuleSet set) { mRuleSet = set; };
	void setPool(vector<Term*> terms) { mPool = terms; };

	vector<Term*> Reduce();

private: // General Utilites
	bool IsSolvable(Term* t1, Operator* op, Term* t2);
	vector<Term*> BreakBracket(Bracket* brack, bool order = false, Identifier_t order_type = Identifier_t::_high_order_left);
	vector<Term*> OrderTerms(vector<Term*> terms, Identifier_t order);
	vector<Term*> getRangeOfTerms(vector<Term*> terms, int begin, int end);

	Bracket* convertToBracket(Term* t1);
	Variable* converToVariable(Term* t1);
	Constant* convertToConstant(Term* t1);

	bool IsHigherSig(Term* t1, Term* t2);

	bool IsEquTerms(Term* t1, Term* t2);
	bool IsEquTypes(Term* t1, Term* t2);
	bool IsEquPower(Term* t1, Term* t2);
	bool IsEquValue(Term* t1, Term* t2);
	bool IsEquVariable(Term* t1, Term* t2);
	bool IsBracket(Term* t1);

private: // Power Evaluation
	vector<Term*> ReducePower(Term* t1);

public: // Addition Evaluation // TODO: PUBLIC AFTER TESTING
	vector<Term*> Add(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

public: // Subtraction Evaluation // TODO: PUBLIC AFTER TESTING
	vector<Term*> Sub(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

public: // Multiplication Evaluation // TODO: PUBLIC AFTER TESTING
	vector<Term*> Mul(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

private:
	QMRuleSet mRuleSet;

	vector<Term*> mPool;
};

#endif // !QMREDUCER_H
