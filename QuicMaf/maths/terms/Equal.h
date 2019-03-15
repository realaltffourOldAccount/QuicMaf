#ifndef EQUAL_H
#define EQUAL_H
#pragma once

using namespace std;
#include "Term.h"
#include "../defines.h"

class Equal : public Term {
public:
	Equal() : Term() {
		mType = TermTypes::Equ;
	}

	string to_str() override {
		return "=";
	}
};

#endif // !EQUAL_H
