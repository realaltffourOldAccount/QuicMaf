#ifndef EQUATIONS_H
#define EQUATIONS_H
#pragma once
#include "defines.h"
#include "terms/Brackets.h"
#include "terms/Constant.h"
#include "terms/Equal.h"
#include "terms/Operator.h"
#include "terms/Term.h"
#include "terms/Variable.h"
#include "terms/Paranthesis.h"
#include "tokenizer.h"

#include "../vendor/lexertk.hpp"

struct Equation {
	vector<Term*> lwing;
	vector<Term*> rwing;

	LOG* log = nullptr;

	// TODO: Optimize this mess
	void Parse(lexertk::generator expression) {
		tok_log = log;

		string lwing_str;
		string rwing_str;

		bool left = true;
		for (int i = 0; i < expression.size(); i++) {
			if (expression[i].value[0] == '\0') continue;

			if (expression[i].value[0] == '=') {
				left = !left;
				continue; // dont add equal sign
			}
			else left = left;
			(left) ? lwing_str.append(expression[i].value) : rwing_str.append(expression[i].value); // append to wings
		}

		// Tokenize wings
		lexertk::generator lwing_lexer;
		lwing_lexer.process(lwing_str);
		lwing = tokenize(lwing_lexer);
		if (lwing.empty()) {
			return; // error occured
		}

		lexertk::generator rwing_lexer;
		rwing_lexer.process(rwing_str);
		rwing = tokenize(rwing_lexer);
		if (lwing.empty()) {
			return; // error occured
		}

		tok_log = nullptr;
	}
};

#endif // !EQUATIONS_H