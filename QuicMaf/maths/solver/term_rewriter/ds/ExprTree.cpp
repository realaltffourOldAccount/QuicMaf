#include "ExprTree.h"

void ExprTree::setPool(vector<Term*> pool) {
	mPool.set(pool);
	mHead = new TreeNode();
}

void ExprTree::GenerateTree() {
	mHead = _genTree(mPool);
}

void ExprTree::print() {
	_print("", mHead, false);
}

TreeNode* ExprTree::_genTree(TermPool pool) {
	if (pool.size() == 0) return nullptr;
	if (pool.size() == 1) {
		TreeNode* node = new TreeNode();
		node->_term = pool[0];
		node->_left = nullptr;
		node->_right = nullptr;
		return node;
	}

	// Number operations
	std::vector<pair<int, int>> operators;
	int counter = 0;

	// DIV, MUL first
	for (int i = 0; i < pool.size(); i++) {
		if (pool[i]->mType == TermTypes::Op) {
			if (pool[i]->mOperator == '/') {
				pair<int, int> resultant;
				resultant.first = i;
				resultant.second = counter++;
				operators.push_back(resultant);
			}
			if (pool[i]->mOperator == '*') {
				pair <int, int> resultant;
				resultant.first = i;
				resultant.second = counter++;
				operators.push_back(resultant);
			}
		}
	}

	// ADD, SUB Second
	for (int i = 0; i < pool.size(); i++) {
		if (pool[i]->mType == TermTypes::Op) {
			if (pool[i]->mOperator == '+') {
				pair<int, int> resultant;
				resultant.first = i;
				resultant.second = counter++;
				operators.push_back(resultant);
			}
			else if (pool[i]->mOperator == '-') {
				pair<int, int> resultant;
				resultant.first = i;
				resultant.second = counter++;
				operators.push_back(resultant);
			}
		}
	}
	stable_sort(operators.begin(), operators.end(),
		[this](pair<int, int> arg1, pair<int, int> arg2) {
		return arg1.second < arg2.second;
	});

	// Split upon op, recurse
	TermPool left, right;
	// dont get the op to left
	auto ptr1 = operators[operators.size() - 1].first;
	left.set(
		pool.getRange(0, ptr1-1)
	);
	// dont get the op to right
	right.set(
		pool.getRange(ptr1 + 1, pool.size()-1)
	);

	// get the op here
	Term* op = pool[left.size()];
	TreeNode* node = new TreeNode();
	node->_term = op;
	node->_left = _genTree(left);
	node->_right = _genTree(right);
	return node;
}

void ExprTree::_print(const std::string prefix, const TreeNode* node, bool isLeft) {
	if (node != nullptr) {
		cout << prefix;

		cout << (isLeft ? "|--" : "^--");

		// print the value of the node
		cout << node->_term->to_str() << endl;

		_print(prefix + (isLeft ? "|   " : "    "), node->_left, true);
		_print(prefix + (isLeft ? "|   " : "    "), node->_right, false);
	}
}
