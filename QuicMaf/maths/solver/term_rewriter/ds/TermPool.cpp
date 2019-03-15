#include "TermPool.h"

void TermPool::set(vector<Term*> terms) {
	this->clear();
	for (auto* term : terms)
		mList.push_back(term);
}

void TermPool::insert(Term * term, int pos) {
	auto begining = std::begin(mList);
	while (pos--) begining++;
	mList.insert(begining, term);
}

void TermPool::insertRange(vector<Term*> terms, int pos) {
	auto begining = std::begin(mList);
	while (pos--) begining++;
	for (auto *term : terms) mList.insert(begining++, term);
}

void TermPool::deleteItem(int pos) {
	auto begining = std::begin(mList);
	while (pos--) begining++;
	mList.remove(*begining);
}

void TermPool::deleteItemRange(int begin, int end) {
	for (int i = begin; i < end; i++)
		this->deleteItem(i);
}

vector<Term*> TermPool::getRange(int begin, int end) {
	vector<Term*> res;
	for (int i = begin; i <= end; i++)
		res.push_back(this->operator[](i));
	return res;
}

void TermPool::push_back(Term * term) {
	mList.push_back(term);
}

void TermPool::push_front(Term * term) {
	mList.push_front(term);
}

Term * TermPool::pop_back() {
	auto res = *mList.end();
	mList.pop_back();
	return res;
}

Term * TermPool::pop_front() {
	auto res = *mList.begin();
	mList.pop_front();
	return res;
}

void TermPool::clear() {
	mList.clear();
}

Term * TermPool::operator[](int index) const {
	auto it = mList.begin();
	std::advance(it, index);
	// 'it' points to the element at index 'N'
	auto res = *it;
	return res;
}

int TermPool::size() const {
	return mList.size();
}

