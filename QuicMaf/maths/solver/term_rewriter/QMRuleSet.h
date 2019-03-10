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
	void push_back(QMRule rule);
	void push_front(QMRule rule);

	void pop_back(QMRule rule);
	void pop_front(QMRule rule);

	void order();

	auto GetRuleSet() { return mRuleSet; }

private:
	deque<QMRule> mRuleSet;
};

#endif // !QMRULESET_H
