#pragma once
#define DEBUG_MODE
#include <iostream>
#include <string>

#include "vendor/lexertk.hpp"

#include "maths/solver/Solver.h"
#include "maths/Equations.h"

#include "maths/solver/term_rewriter/QMEvalHelper.h"
#include "maths/solver/term_rewriter/QMEvaluator.h"

#include "maths/solver/term_rewriter/ds/ExprTree.h"

#define MAIN_APP
//#define APP_TEST


#ifdef MAIN_APP

using namespace std;

int main() {

	while (true) {
		cout << "Enter an equation: ";
		string input;
		cin >> input;

		cout << "Solution: " << endl;

		Solver solver;
		solver.Parse(input);
		solver.Solve();

		for (auto *lwing : solver.mEquation->lwing)
			cout << lwing->to_str();
		cout << "=";
		for (auto *rwing : solver.mEquation->rwing)
			cout << rwing->to_str();

		cout << endl;

		system("PAUSE");
		system("CLS");
	}


	return true;
}
#endif // MAIN_APP

#ifdef APP_TEST

int main() {
	lexertk::generator lexer;
	lexer.process("12341-(2x+3)");

	auto result = tokenize(lexer);

	Solver solver;
	solver.Parse("4+3=4x+6");
	solver.Solve();

	for (auto *lwing : solver.mEquation->lwing)
		cout << lwing->to_str();
	cout << "=";
	for (auto *rwing : solver.mEquation->rwing)
		cout << rwing->to_str();

	cout << endl;

	system("PAUSE");
	return 0;
}

#endif