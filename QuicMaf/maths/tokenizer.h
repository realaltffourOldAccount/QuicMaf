#ifndef TOKENIZER_H
#define TOKENIZER_H
#pragma once
#include "defines.h"
#include "terms/Brackets.h"
#include "terms/Constant.h"
#include "terms/Equal.h"
#include "terms/Operator.h"
#include "terms/Term.h"
#include "terms/Variable.h"
#include "terms/Paranthesis.h"

#include "../vendor/lexertk.hpp"

using namespace std;

struct Token {
	int begin = 0;
	int end = 0;
	Token(int b, int e) : begin(b), end(e) {}
	Token() {}
};

static string retrieveSubString(string str, Token token) {
	string res;
	for (int i = token.begin; i <= token.end; i++)
		if (str[i] != '\0') // ignore null terminator
			res.push_back(str[i]);
	return res;
}

static lexertk::generator retriveSubLexer(lexertk::generator gen, Token tok) {
	lexertk::generator res;
	vector<string> str;
	for (int i = tok.begin; i <= tok.end; i++)
		str.push_back(gen[i].value);

	string val;
	for (int i = 0; i < str.size(); i++)
		val.append(str[i]);

	res.process(val);
	return res;
}

//struct Paranthesis {
//	bool isOpening = true;
//	int pos = -1;
//
//	Paranthesis(bool is, int _pos) : isOpening(is), pos(_pos) {}
//};

vector<Term*> tokenize(lexertk::generator lexed);

static Bracket* tokenize_bracket(lexertk::generator gen, Token* token, string coefficient) {
	Bracket* result = nullptr;
	result = new Bracket();

	lexertk::generator bracks;


	// DETERMINE THE ENDING OF THE BRACKETS
	int counter = 0;
	bool state = false;
	int index = token->begin;
	do {
		if (isBracketsOpening(gen[index].value[0])) {
			counter++;
			state = true;
		}
		else if (isBracketsClosing(gen[index].value[0])) {
			counter--;
			state = false;
		}
		index++;
	} while (!(!state && counter == 0));

	bracks = retriveSubLexer(gen, Token(token->begin, index-1));
	token->end = index - 1; // to make sure we move the token pointer to the end of bracks

	// DELETE THIS BRACKET PARANTHESIS
	bracks = retriveSubLexer(bracks, Token(1, bracks.size()-2));

	// Tokenize its term
	// first make sure it is not empty
	if (bracks.empty()) {
		cout << "Brackets can't be empty!" << endl;
		system("PAUSE");
		exit(0);
	}
	// tokenize terms
	auto terms = tokenize(bracks);

	result->mTerms = terms;

	// ADD COEFFICIENT TO RESULT
	if (coefficient != "") {
		lexertk::generator lex;
		lex.process(coefficient);
		result->setConstant(tokenize(lex)[0]);
	}
	return result;
}

static vector<Term*> tokenize(lexertk::generator lexed) {
	vector<Term*> result;

	for (int i = 0; i < lexed.size(); i++) {
		auto lex = lexed[i];
		auto after_lex = lexed[i + 1];
		Token tok;
		tok.begin = i;

		if (is_all_digits(lex.value)) {
			// number

			// check for variables
			if (isalpha(after_lex.value[0])) {
				// variable detected

				// check for power
				// if so read the power and its constant
				if (isPower(lexed[i + 2].value[0])) {
					// powers ONLY can be numbers no evaluation is done in the power
					// ex: 5^2*3 // the expression will be 5 by 5 then multiply 3
					if (!is_all_digits(lexed[i + 3].value)) {
						cout << "ONLY numbers are allowed in powers!" << endl;
						system("PAUSE");
						exit(0);
						/////// ENDING OF TREE
					}

					Variable* Var = nullptr;
					Var = new Variable(atof(&lex.value[0]), after_lex.value[0], atof(&lexed[i + 3].value[0]));
					result.push_back(Var);
					tok.end = i + 3;
					/////// ENDING OF TREE
				}
				else {
					// The variable is simple!
					Variable* Var = nullptr;
					Var = new Variable(atof(&lex.value[0]), after_lex.value[0]);
					result.push_back(Var);
					tok.end = i + 1;
				}
			}
			else if (isBrackets(after_lex.value[0]) && isBracketsOpening(after_lex.value[0])) {
				// check for brackets,
				// if so tokenize the brackets
				tok.begin++; // consume the coefficient
				result.push_back(tokenize_bracket(lexed, &tok, lex.value));
				/////// ENDING OF TREE
			}
			else if (isPower(after_lex.value[0])) {
				// check for powers,
				// if so read the power and its constant
				// powers ONLY can be numbers no evaluation is done in the power
				// ex: 5^2*3 // the expression will be 5 by 5 then multiply 3
				if (!is_all_digits(lexed[i + 2].value)) {
					cout << "ONLY numbers are allowed in powers!" << endl;
					system("PAUSE");
					exit(0);
					/////// ENDING OF TREE
				}

				Constant* Const = nullptr;
				Const = new Constant(atof(&lex.value[0]), atof(&lexed[i + 2].value[0]));
				result.push_back(Const);
				tok.end = i + 2;
				/////// ENDING OF TREE
			}
			else {
				// The number is simple!
				Constant* Const = nullptr;
				Const = new Constant(atof(&lex.value[0]));
				result.push_back(Const);
				tok.end = i;
				/////// ENDING OF TREE
			}
		}
		else if (isalpha(lex.value[0])) {
			// variable

			// check for power
			// if so read the power and its constant
			if (isPower(after_lex.value[0])) {
				// powers ONLY can be numbers no evaluation is done in the power
				// ex: 5^2*3 // the expression will be 5 by 5 then multiply 3
				if (!is_all_digits(lexed[i + 2].value)) {
					cout << "ONLY numbers are allowed in powers!" << endl;
					system("PAUSE");
					exit(0);
					/////// ENDING OF TREE
				}

				Variable* Var = nullptr;
				Var = new Variable(1.0, lex.value[0], lexed[i + 2].value[0]);
				result.push_back(Var);
				tok.end = i + 2;
				/////// ENDING OF TREE
			}
			else {
				// The variable is simple!
				Variable* Var = nullptr;
				Var = new Variable(1.0, lex.value[0]);
				result.push_back(Var);
				tok.end = i;
				/////// ENDING OF TREE
			}
		}
		else if (isBracketsOpening(lex.value[0])) {
			// bracket
			result.push_back(tokenize_bracket(lexed, &tok, ""));
		}
		else if (isArithmitic(lex.value[0])) {
			// operator

			Operator *op = nullptr;
			op = new Operator(lex.value[0]);
			result.push_back(op);
			tok.end = i;
		}
		else if (isEqualChar(lex.value[0])) {
			// equal sign

			Equal* equ = nullptr;
			equ = new Equal();
			result.push_back(equ);
			tok.end = i;
		}

		i = tok.end; // no need to increment, automatically done in loop statment
	}

	return result;
}
#endif // !TOKENIZER_H
