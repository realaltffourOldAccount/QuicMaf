#ifndef QMRULESET_H
#define QMRULESET_H
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

class QMRuleSet {
public:
	void push_back(QMRule rule) { mRuleSet.push_back(rule); }
	void push_front(QMRule rule) { mRuleSet.push_front(rule); }

	void pop_back() { mRuleSet.pop_back(); };
	void pop_front() { mRuleSet.pop_front(); }

	QMRule back() { return mRuleSet[mRuleSet.size() - 1]; }
	QMRule front() { return mRuleSet[0]; }

	void order() {
		stable_sort(mRuleSet.begin(), mRuleSet.end());
	}

	int size() { return mRuleSet.size(); }

	auto GetRuleSet() { return mRuleSet; }

	inline QMRule& operator[](int index) {
		return mRuleSet[index];
	}
private:
	deque<QMRule> mRuleSet;
};

#endif // !QMRULESET_H
