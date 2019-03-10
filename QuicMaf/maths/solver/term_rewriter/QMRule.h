#ifndef QMRULE_H
#define QMRULE_H
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

enum Identifier_t {
	_const = 1,
	_var = 2,
	_brack,
	_eval,
	_add,
	_sub,
	_mul,
	_div,
	_equ,
	_const_aval,
	_var_aval,
	_pow_diff,
	_no_pow_diff,
	_var_diff,
	_no_var_diff,
	_high_order_left,
	_high_order_right,
	_priority1,
	_priority2,
	_priority3,
	_priority4,
	_priority5,
};

class QMRule {
public:
	QMRule(vector<int> rule, vector<int> props, int prio)
		: mRules(rule), mProps(props), mPrior(prio) {}

	vector<int> GetRules() const { return mRules; }
	vector<int> GetProps() const { return mProps; }
	int GetPrior() { return mPrior; }

	bool operator==(const QMRule& left) const {
		if (left.mPrior != mPrior) return false;
		if (left.mProps != mProps) return false;
		if (left.mRules != mRules) return false;
		return true;
	}

	bool operator>(const QMRule& left) const {
		return (left.mPrior > mPrior);
	}
	bool operator<(const QMRule& left) const {
		return (left.mPrior < mPrior);
	}

	vector<Term*> Reduce(vector<Term*> terms); // applies self rule on terms vector, only once
private:
	vector<Term*> Evaluate();	// evaluation method
	

	vector<int> mRules;
	vector<int> mProps;
	int mPrior;
};
#endif // QMRULE_H
