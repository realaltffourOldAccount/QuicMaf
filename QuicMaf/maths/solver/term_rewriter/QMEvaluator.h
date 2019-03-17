#ifndef QMEVALUATOR_H
#define QMEVALUATOR_H
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
#include "ds/ExprTree.h"
#include "ds/TermPool.h"

static TermPool mPool;
static vector<Term*> solveNode(TreeNode * node) {
	if (node->_term->mType != TermTypes::Op) return { node->_term };

	vector<Term*> res;
	auto op = node->_term;
	auto t1 = solveNode(node->_left);
	auto t2 = solveNode(node->_right);

	Term* t1_final = nullptr;
	Term* t2_final = nullptr;

	if (t1.size() > 1) {
		// convert it to bracket
		Bracket* brack = new Bracket();
		for (auto *term : t1) brack->mTerms.push_back(term);
		t1_final = brack;
	}
	else t1_final = t1[0];

	if (t2.size() > 1) {
		// convert it to bracket
		Bracket* brack = new Bracket();
		for (auto *term : t2) brack->mTerms.push_back(term);
		t2_final = brack;
	}
	else t2_final = t2[0];

	if (op->mOperator == '*')
		res = QMEvalHelper::Mul(t1_final, t2_final);
	if (op->mOperator == '/')
		res = QMEvalHelper::Div(t1_final, t2_final);
	if (op->mOperator == '+')
		res = QMEvalHelper::Add(t1_final, t2_final);
	if (op->mOperator == '-')
		res = QMEvalHelper::Sub(t1_final, t2_final);
	return res;
}
static void evaluate() {
	// Step 1:
	// Reduce Powers except for variables
	for (int i = 0; i < mPool.size(); i++)
		if (mPool[i]->mType == TermTypes::Const)
			*mPool[i] = *QMEvalHelper::ReducePower(mPool[i])[0];

	// Step 2:
	// Remove begining sign, if any
	if (mPool[0]->mType == TermTypes::Op) {
		auto op = mPool.pop_back(); // remove op
		auto res = *mPool.begin();
		res->mValue = res->mValue * (op->mOperator == '+') ? 1 : -1; // multiply

		mPool.pop_front(); // remove term going to be modified
		mPool.push_front(res);
	}

	// Step 3:
	// Generate ExpressionTree
	ExprTree tree;
	tree.setPool(mPool.vec());
	tree.GenerateTree();

	// Step 4: Evaluate!!!
	mPool.set(solveNode(tree.getHead()));
}
static vector<Term*> getPool() {
	vector<Term*> res;
	for (int i = 0; i < mPool.size(); i++)
		res.push_back(mPool[i]);
	return res;
}

static vector<Term*> evaluate(vector<Term*> term) {
	mPool.set(term);
	evaluate();
	return getPool();
}

#endif // !QMEVALUATOR_H
