#ifndef TERM_PARSER_H
#define TERM_PARSER_H
#pragma once
#include "../defines.h"
using namespace std;

/*
	The goal of this class is to tokenize a string of equation and splits
	it to mathematical term, with brackets begin a complete terms alone.
	Ex:
	2(5 + 9) - 56=90
	If the above input was given to the main function in this class it must give an output of:
	vector:
		- 2(5 + 9)
		- -
		- 56
		- =
		- 90
*/

struct Token {
	unsigned int begin = 0;
	unsigned int end = 0;

};

string retrieveSubString(string str, Token token) {
	string res;
	for (int i = token.begin; i < token.end; i++) 
		res.push_back(str[i]);
	return res;
}

struct Paranthesis {
	bool isOpening = false;
	int index;
	Paranthesis(bool is, int i) : isOpening(is), index(i) {}
};

vector<string> split_terms(string str) {
	vector<string> result;

	for (int i = 0; i < str.size(); i++) { // iterate token wise.
		Token token;
		token.begin = i;
		token.end = -1;
		if (str[i] == '(') {
			// expecting a bracket token
			
			// make bracket for evaluation of balance
			vector<Paranthesis> brackets;
			brackets.push_back(Paranthesis(true, i));
			for (int j = i + 1; j < str.size(); j++)
				if (str[j] == ')')
					brackets.push_back(Paranthesis(false, j));
				else if (str[j] == '(')
					brackets.push_back(Paranthesis(true, j));

			// Cant be empty
			if (brackets.empty()) {
				cout << "[PARSER][BRACKETS] Brackets stack can't be empty! Probably a bug!" << endl;
				system("PAUSE");
				exit(1);
			}

			// check for bracket balance
			int brack_cnt = 0;
			for (auto bracket : brackets)
				if (bracket.isOpening)brack_cnt++;
				else brack_cnt--;
			
			if (brack_cnt == 0) {
				token.end = brackets[brackets.size() - 1].index+1;
				// add one to compensate for the < and not <= in the retrievesubstring
			}
			else {
				cout << "[PARSER][BRACKET] Brackets aren't balanced!" << endl;
				system("PAUSE");
				exit(1);
			}
		}
		else if (isdigit(str[i])) {
			// expecting a digit token.
			
			// consume till the end of the digits
			int endofconst = i; // points to the current char
			while (isdigit(str[++endofconst])); // endofconst will have the index of the end of the term.
												// ENDOFCONST WILL POINT ONE INDEX PLUS

			// check if there is a var
			if (isalpha(str[endofconst])) endofconst++;

			// check for the power
			if (str[endofconst] == '^') {
				// consume till the end of the constant
				while (isdigit(str[++endofconst]));
			}

			token.end = endofconst;
		}
		else if (isalpha(str[i])) {
			// expecting a variable token.
			
			int endofvar = i;
			// check for the power
			if (str[endofvar + 1] == '^') {
				// consume till the end of the digits
				endofvar++; // point the index to '^'
				while (isdigit(++endofvar)); // endofconst wll have the index of the end of the term.
											 // ENDOFCONST WILL POINT ONE INDEX PLUS
			}

			token.end = endofvar+1; // add one incase it was only a var
									// to compensate over the < not <= in the retrievesubstring
		}
		else if (str[i] == ' ') {
			// a space detected.
			continue; // skip spaces
		}
		else {
			// expecting either an equal or any of the arithmetic.
			token.end = i+1; // the token is only one letter.
			// add one incase it was only a char
			// to compensate over the < not <= in the retrievesubstring
		}
		i = token.end-1;
		result.push_back(retrieveSubString(str, token));
	}
	return result;
}

#endif // !

