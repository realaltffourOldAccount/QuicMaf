#pragma once
#define DEBUG_MODE
#include <iostream>
#include <string>

#include "vendor/lexertk.hpp"

#include "maths/solver/Solver.h"
#include "maths/Equations.h"

#include "maths/solver/term_rewriter/QMReducer.h"

//#define MAIN_APP
#define APP_TEST


#ifdef MAIN_APP

using namespace std;


int main() {

	while (true) {
		cout << "Enter an equation: ";
		string input;
		cin >> input;

		lexertk::generator generator;

		if (!generator.process(input)) {
			cout << "Failed to lex: " << input << endl;
			system("PAUSE");
			system("CLS");
			return true;
		}

		//lexertk::helper::commutative_inserter ci;
		//ci.process(generator);

		lexertk::helper::bracket_checker bc;
		bc.process(generator);

		if (!bc.result()) {
			cout << "Failed Bracket Check!" << endl;
			system("PAUSE");
			system("CLS");
			return 1;
		}

#ifdef DEBUG_MODE
		lexertk::helper::dump(generator);
#endif // DEBUG_MODE

		Equation equation;
		equation.Parse(generator);


		system("PAUSE");
		system("CLS");
	}


	return true;
}
#endif // MAIN_APP

#ifdef APP_TEST


int main() {
	lexertk::generator lexer;
	lexer.process("4x/(2+2x)");

	auto result = tokenize(lexer);

	QMReducer reducer;
	reducer.setPool(result);
	auto frac = static_cast<Fraction*>(result[0]);

	auto res = reducer.Div(frac->mNomin[0], frac->mDomin[0]);

	return 0;
}

#endif