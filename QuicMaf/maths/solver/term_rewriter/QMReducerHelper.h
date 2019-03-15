#ifndef QMREDUCER_H
#define QMREDUCER_H
#pragma once
// WARINING: Only one variable is allowed or errors will happen!

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

struct IdsWithTerms {
	Identifier_t _id;
	Term* _term;
};
class QMEvalHelper {
public: // General Utilites
	static Identifier_t getIdenByTerm(Term* term) {
		if (term->mType == TermTypes::Const) return Identifier_t::_const;
		if (term->mType == TermTypes::Var) return Identifier_t::_var;
		if (term->mType == TermTypes::Brack) return Identifier_t::_brack;
		if (term->mType == TermTypes::Op) {
			if (term->mOperator == '+') return Identifier_t::_add;
			if (term->mOperator == '-') return Identifier_t::_sub;
			if (term->mOperator == '*') return Identifier_t::_mul;
			if (term->mOperator == '/') return Identifier_t::_div;
		}
	}
	static vector<IdsWithTerms> convertRangeToIds(vector<Term*> range);

	static bool IsSolvable(Term* t1, Operator* op, Term* t2);
	static vector<Term*> BreakBracket(Bracket* brack, bool order = false, Identifier_t order_type = Identifier_t::_high_order_left);
	static vector<Term*> OrderTerms(vector<Term*> terms, Identifier_t order);
	static vector<Term*> getRangeOfTerms(vector<Term*> terms, int begin, int end);
	static vector<Identifier_t*> getRangeOfIden(vector<Identifier_t*> terms, int begin, int end);

	static Bracket* convertToBracket(Term* t1);
	static Variable* convertToVariable(Term* t1);
	static Constant* convertToConstant(Term* t1);

	static bool IsHigherSig(Term* t1, Term* t2);

	static bool IsEquTerms(Term* t1, Term* t2);
	static bool IsEquTypes(Term* t1, Term* t2);
	static bool IsEquPower(Term* t1, Term* t2);
	static bool IsEquValue(Term* t1, Term* t2);
	static bool IsEquVariable(Term* t1, Term* t2);
	static bool IsBracket(Term* t1);

public: // Power Evaluation
	static vector<Term*> ReducePower(Term* t1);

public: // Addition Evaluation
	static vector<Term*> Add(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

public: // Subtraction Evaluation
	static vector<Term*> Sub(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

public: // Multiplication Evaluation
	static vector<Term*> Mul(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);

public:	// Division Evaluation
	static bool IsDivSolvable(Term* t1, Term* t2);
	static bool IsDivSpecialCase(Term* t1, Term* t2);

	static vector<Term*> gcdofTerms(Term* t1, Term* t2);
	static vector<Term*> FactorizeTermsToBrack(vector<Term*> terms, vector<Term*> terms2);
	static vector<Term*> FactorizeTermsToBrack(vector<Term*> terms);

	static bool TermsMatch(vector<Term*> terms1, vector<Term*> terms2);

	static vector<Term*> Div(Term* t1, Term* t2, Identifier_t order = Identifier_t::_high_order_left);
};

#endif // !QMREDUCER_H
