#pragma once
#define DEBUG_MODE
#include <iostream>
#include <string>

#include "vendor/lexertk.hpp"

#include "maths/tokenizer.h"

#define MAIN_APP
//#define APP_TEST


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


		auto result = tokenize(generator);

		system("PAUSE");
		system("CLS");
	}


	return true;
}
#endif // MAIN_APP

#ifdef APP_TEST



#endif