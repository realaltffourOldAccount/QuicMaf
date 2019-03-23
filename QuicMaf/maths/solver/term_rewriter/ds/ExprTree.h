#ifndef EXPR_TREE_H
#define EXPR_TREE_H
#pragma once
#include "../../../Equations.h"
#include "../../../defines.h"
#include "../../../terms/Brackets.h"
#include "../../../terms/Constant.h"
#include "../../../terms/Equal.h"
#include "../../../terms/Operator.h"
#include "../../../terms/Term.h"
#include "../../../terms/Variable.h"
#include "../../../terms/Paranthesis.h"
#include "../../../tokenizer.h"

#include "../../../../vendor/lexertk.hpp"
#include "../QMEvalHelper.h"
#include "TermPool.h"

struct TreeNode {
	Term* _term;
	TreeNode* _left = nullptr;
	TreeNode* _right = nullptr;
};

class ExprTree {
public:
	void setPool(vector<Term*> pool);
	void GenerateTree();

	void print();

	TreeNode* getHead() { return mHead; }

private:
	TreeNode* _genTree(TermPool pool);
	void _print(const std::string prefix, const TreeNode* node, bool isLeft);

	TreeNode* mHead = nullptr;
	TermPool mPool;
};


class ExprTreeReversed {
public:
	void setPool(vector<Term*> pool);
	void GenerateTree();

	void print();

	TreeNode* getHead() { return mHead; }

private:
	TreeNode* _genTree(TermPool pool);
	void _print(const std::string prefix, const TreeNode* node, bool isLeft);

	TreeNode* mHead = nullptr;
	TermPool mPool;
};




#endif // !EXPR_TREE_H
