#ifndef EQUAL_H
#define EQUAL_H
#pragma once

using namespace std;
#include "Term.h"
#include "../defines.h"

class Equal : public Term {
public:
	Equal() {
		mType = TermTypes::Equ;
	}
};

#endif // !EQUAL_H
