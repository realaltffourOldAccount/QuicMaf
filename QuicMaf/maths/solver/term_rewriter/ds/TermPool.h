#ifndef TERMPOOL_H
#define TERMPOOL_H
#pragma once
#include <list>
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


class TermPool {
public:
	void set(vector<Term*> terms);
	void insert(Term* term, int pos = -1);
	void insertRange(vector<Term*> terms, int pos = -1);
	void deleteItem(int pos);
	void deleteItemRange(int begin, int end);
	vector<Term*> getRange(int begin, int end);
	void push_back(Term* term);
	void push_front(Term* term);
	Term* pop_back();
	Term* pop_front();

	auto begin() { return mList.begin(); }
	auto end() { return mList.end(); }

	void clear();

	Term* operator[](int index) const;
	auto getIterator(int index) {
		auto beginining = std::begin(mList);
		std::advance(beginining, index);
		return beginining;
	}

	int size() const;
	void order() {
		mList.sort();
	}

	vector<Term*> vec() {
		vector<Term*> res;
		for (auto *item : mList)
			res.push_back(item);
		return res;
	}
private:
	list<Term*> mList;
};


#endif // !TERMPOOL_H
