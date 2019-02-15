#ifndef EQUATION_H
#define EQUATION_H
#pragma once

using namespace std;
#include "terms/Term.h"
#include "terms/Brackets.h"
#include "terms/Constant.h"
#include "terms/Operator.h"
#include "terms/Variable.h"
#include "terms/Equal.h"

#include "terms/term_parser.h"

class Equation {
public:
	vector<Term*> lwing;
	vector<Term*> rwing;

	string Parse(std::string equation) {

		/* ALL OF THE COMMENTS GOT FROM THE NOTES.PARSING.ALGORITHM */

		// - split the terms in the term of spaces
		auto semi_terms = split(equation, " ");

		bool lwing_turn = true;
		// - iterate over the terms
		for (int i = 0; i < semi_terms.size(); i++) {
			// , call the iterator mTerm:
			auto term = semi_terms[i];
			// - if there exists an opening bracket in the mTerm
			if (isContain(term, '(')) {
				// find the closing.
				vector<string> terms_with_cbrack;
				for (; i < semi_terms.size(); i++) {
					if (isContain(semi_terms[i], ')')) {
						terms_with_cbrack.push_back(semi_terms[i]);
						break;
					}
					terms_with_cbrack.push_back(semi_terms[i]);
				}
				auto bracket = parseBracket(term, terms_with_cbrack);
				(lwing_turn) ? lwing.push_back(bracket) : rwing.push_back(bracket);
				continue;
			}
			auto _term = convertNormalToTerm(term);
			if (_term->GetType() == TermTypes::Equ) {
				lwing_turn = !lwing_turn;
				continue;
			}

			(lwing_turn) ? lwing.push_back(_term) : rwing.push_back(_term);
		}

		return equation;
	}
public:
	Bracket* parseBracket(string str1, vector<string> terms_with_cbrack) {
		Bracket *bracket = nullptr;
		bracket = new Bracket();

		auto splitted = split(str1, "(");
		string coefficient = splitted[0];

		// convert coefficient to a term
		if (coefficient != "")
		bracket->setConstant((Term*)convertNormalToTerm(coefficient));

		if (splitted.size() > 1) {
			for (int i = 1; i < splitted.size(); i++) {
				auto term = (Term*)convertNormalToTerm(splitted[i]);
				bracket->mTerms.push_back(term);
			}
		}

		// terms:
		for (int i = 1; i < terms_with_cbrack.size(); i++) {
			auto term = terms_with_cbrack[i];
			if (isContain(term, ')')) {
				term.erase(indexofChar(term, ')'));
				bracket->mTerms.push_back(convertNormalToTerm(term));
				break;
			}
			bracket->mTerms.push_back(convertNormalToTerm(term));
		}

		return bracket;
	}

	Term* convertNormalToTerm(string str) {
		if (isContain(str, '^')) {
			auto coeffic_terms = split(str, "^");
			if (isContainAlph(str)) {
				auto terms = split(str, "^");
				Variable *var = nullptr;
				var = new Variable(atof(&terms[0][0]), terms[0][indexofAlph(terms[0])], atof(&terms[1][0]));
				return var;
			}
			else if (is_all_digits(coeffic_terms[0]) && is_all_digits(coeffic_terms[1])) {
				Constant *Const = nullptr;
				Const = new Constant(atof(&coeffic_terms[0][0]), atof(&coeffic_terms[1][0]));
				return Const;
			}
		}
		else if (is_all_digits(str)) {
			Constant *Const = nullptr;
			Const = new Constant(atof(&str[0]));
			return Const;
		}
		else if (isContainAlph(str)) {
			Variable *var = nullptr;
			var = new Variable(atof(&str[0]), str[indexofAlph(str)]);
			return var;
		}
		else if (isArithmitic(str[0])) {
			Operator *op = nullptr;
			op = new Operator(str[0]);
			return op;
		}
		else if (isEqualChar(str[0])) {
			Equal* equ = nullptr;
			equ = new Equal();
			return equ;
		}
	}
};
#endif // !EQUATION_H
