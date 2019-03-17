#include "Solver.h"

void Solver::Parse(string equation) {
	mEquation = new Equation();
	lexertk::generator lexer;
	lexer.process(equation);
	mEquation->Parse(lexer);
}

Equation * Solver::Solve() {
	// Put vars on one side and consts on another
	mEquation = PutVarsInOneSide(mEquation);
	mEquation = PutConstsInOneSide(mEquation);

	// Evaluate lwing and rwing
	mEquation->lwing = evaluate(mEquation->lwing);
	mEquation->rwing = evaluate(mEquation->rwing);

	// Check For Diversion Calculations
	mEquation = ApplyDiversionCalcs(mEquation);

	return mEquation;
}

vector<Term*> Solver::FlipTermSigns(vector<Term*> terms) {
	vector<Term*> res;

	terms = RemNonEssientalOps(terms);

	for (int i = 0; i < terms.size(); i++) {
		auto term = terms[i];
		if (term->mType == TermTypes::Brack) {
			auto brack = static_cast<Bracket*>(term);
			if (brack->mConstant == nullptr) {
				brack->mConstant = new Constant(-1, 1);
			}
			else 
			brack->mConstant->mValue = -brack->mConstant->mValue;
			res.push_back(brack);
		}
		else {
			term->mValue = -term->mValue;
			res.push_back(term);
		}
	}

	res = AddNonEssientalOps(terms);
	return res;
}

Equation * Solver::ApplyDiversionCalcs(Equation * equ) {
	// Situations
	// 2x = 50 -> x = 50/2, applies only if one var

	auto lwing = RemNonEssientalOps(equ->lwing);
	auto rwing = RemNonEssientalOps(equ->rwing);

	if (lwing.size() == 1) {
		// Check if x's coeffiecent is higher than 1 or lower
		if (lwing[0]->mValue != 1 && lwing[0]->mPower == 1) {
			// situation of "2x = 50 -> x = 50/2" is confirmed
			auto domin = lwing[0]->mValue;
			Bracket* brack = new Bracket();
			for (int i = 0; i < rwing.size(); i++)
				brack->mTerms.push_back(rwing[i]);
			auto term = QMEvalHelper::Div(brack, new Constant(domin));
			rwing.clear();
			rwing = term;
			lwing[0]->mValue = 1;
		}

	}

	equ->lwing = lwing;
	equ->rwing = rwing;
	return equ;
}

bool Solver::CheckValidOps(Equation * equ) {

	// Check Lwing
	for (int i = 0; i < equ->lwing.size() - 1; i++) {
		auto term = equ->lwing[i];
		if (term->mType == TermTypes::Op) {
			if (i == 0) {
				if (term->mOperator == '/' || term->mOperator == '*') {
					cout << "Can't have division or multiplication at the begining!" << endl;
					system("PUASE");
					exit(0);
				}
				continue;
			}

			auto before_Term = equ->lwing[i - 1];
			auto after_Term = equ->lwing[i + 1];

			if (before_Term->mType == TermTypes::Op ||
				after_Term->mType == TermTypes::Op) {
				cout << "Can't have trialing operators!" << endl;
				system("PUASE");
				exit(0);
			}
		}
	}

	// Check Rwing
	for (int i = 0; i < equ->rwing.size() - 1; i++) {
		auto term = equ->rwing[i];
		if (term->mType == TermTypes::Op) {
			if (i == 0) {
				if (term->mOperator == '/' || term->mOperator == '*') {
					cout << "Can't have division or multiplication at the begining!" << endl;
					system("PUASE");
					exit(0);
				}
				continue;
			}

			auto before_Term = equ->rwing[i - 1];
			auto after_Term = equ->rwing[i + 1];

			if (before_Term->mType == TermTypes::Op ||
				after_Term->mType == TermTypes::Op) {
				cout << "Can't have trialing operators!" << endl;
				system("PUASE");
				exit(0);
			}
		}
	}


	return true;
}

Equation * Solver::PutVarsInOneSide(Equation * equ) {
	Equation* res = new Equation();

	vector<Term*> lwing = OrderTermsByValue(equ->lwing);
	vector<Term*> rwing = OrderTermsByValue(equ->rwing);

	lwing = RemNonEssientalOps(lwing);
	rwing = RemNonEssientalOps(rwing);

	vector<int> lwing_vars;
	vector<int> rwing_vars;

	// Find vars and place them in lwing_vars or rwing_vars
	for (int i = 0; i < lwing.size(); i++) {
		if (lwing[i]->mType == TermTypes::Bund) {
			// check if it has a variable
			Bundle* bundle = static_cast<Bundle*>(lwing[i]);
			bool contains = false;
			for (auto *term : bundle->mTerms)
				if (term->mType == TermTypes::Var) { contains = true; continue; }

			if (contains)
				lwing_vars.push_back(i);
			continue;
		}

		if (lwing[i]->mType == TermTypes::Var)
			lwing_vars.push_back(i);
	}
	for (int i = 0; i < rwing.size(); i++) {
		if (rwing[i]->mType == TermTypes::Bund) {
			// check if it has a variable
			Bundle* bundle = static_cast<Bundle*>(rwing[i]);
			bool contains = false;
			for (auto *term : bundle->mTerms)
				if (term->mType == TermTypes::Var) { contains = true; continue; }

			if (contains)
				rwing_vars.push_back(i);
			continue;
		}

		if (rwing[i]->mType == TermTypes::Var)
			rwing_vars.push_back(i);
	}

	// place variables
	for (auto lwing_term : lwing_vars)
		res->lwing.push_back(lwing[lwing_term]);

	for (auto rwing_term : rwing_vars) {
		auto term = rwing[rwing_term];
		term->mValue = -term->mValue;
		res->lwing.push_back(term);
	}

	// place other terms
	for (int i = 0; i < lwing.size(); i++) {
		if (std::find(lwing_vars.begin(), lwing_vars.end(), i) != lwing_vars.end()) {
			// variable encouterd
			continue;
		}

		res->lwing.push_back(lwing[i]);
	}
	for (int i = 0; i < rwing.size(); i++) {
		if (std::find(rwing_vars.begin(), rwing_vars.end(), i) != rwing_vars.end()) {
			// variable encouterd
			continue;
		}

		res->rwing.push_back(rwing[i]);
	}

	res->lwing = AddNonEssientalOps(res->lwing);
	res->rwing = AddNonEssientalOps(res->rwing);

	return res;
}
Equation * Solver::PutConstsInOneSide(Equation * equ) {
	Equation* res = new Equation();

	vector<Term*> lwing = OrderTermsByValue(equ->lwing);
	vector<Term*> rwing = OrderTermsByValue(equ->rwing);

	lwing = RemNonEssientalOps(lwing);
	rwing = RemNonEssientalOps(rwing);

	vector<int> lwing_const;
	vector<int> rwing_const;

	// Make lwing_const
	for (int i = 0; i < lwing.size(); i++) {
		if (lwing[i]->mType == TermTypes::Bund) {
			// check if it has a constant
			Bundle* bundle = static_cast<Bundle*>(lwing[i]);
			bool contains = false;
			for (auto *term : bundle->mTerms)
				if (term->mType == TermTypes::Const) { contains = true; continue; }

			if (contains)
				lwing_const.push_back(i);
			continue;
		}
		if (lwing[i]->mType == TermTypes::Const)
			lwing_const.push_back(i);
	}
	// Make rwing_const
	for (int i = 0; i < rwing.size(); i++) {
		if (rwing[i]->mType == TermTypes::Bund) {
			// check if it has a cosnstant
			Bundle* bundle = static_cast<Bundle*>(rwing[i]);
			bool contains = false;
			for (auto *term : bundle->mTerms)
				if (term->mType == TermTypes::Const) { contains = true; continue; }

			if (contains)
				rwing_const.push_back(i);
			continue;
		}
		if (rwing[i]->mType == TermTypes::Const)
			rwing_const.push_back(i);
	}

	// Place lwing constants in res->rwing
	for (auto i : lwing_const) {
		auto term = lwing[i];
		term->mValue = -term->mValue;
		res->rwing.push_back(term);
	}
	// Place rwing constants in res->rwing
	for (auto i : rwing_const)
		res->rwing.push_back(rwing[i]);

	// Place other terms of lwing to res->lwing
	for (int i = 0; i < lwing.size(); i++) {
		if (std::find(lwing_const.begin(), lwing_const.end(), i) != lwing_const.end()) {
			// constant encouterd
			continue;
		}
		res->lwing.push_back(lwing[i]);
	}

	// Place other terms of rwing to res->rwing
	for (int i = 0; i < rwing.size(); i++) {
		if (std::find(rwing_const.begin(), rwing_const.end(), i) != rwing_const.end()) {
			// constant encouterd
			continue;
		}
		res->rwing.push_back(rwing[i]);
	}

	res->lwing = AddNonEssientalOps(res->lwing);
	res->rwing = AddNonEssientalOps(res->rwing);

	return res;
}

vector<Term*> Solver::RemNonEssientalOps(vector<Term*> terms) {
	vector<Term*> res;
	for (int i = 0; i < terms.size(); i++) {
		auto term = terms[i];
		if (term->mType == TermTypes::Op) {
			if (term->mOperator == '+') {
				auto after_term = terms[i + 1];
				after_term->mValue = abs(after_term->mValue);
				res.push_back(after_term);
				i++;
				continue;
			}
			if (term->mOperator == '-') {
				auto after_term = terms[i+1];
				after_term->mValue = after_term->mValue * -1;
				res.push_back(after_term);
				i++;
				continue;
			}
			
			if (term->mOperator == '*') {
				auto before_term = *res.end();
				res.pop_back();
				auto after_term = terms[i + 1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(before_term);
				bundle->mTerms.push_back(term);
				bundle->mTerms.push_back(after_term);
				res.push_back(bundle);
				i++;
				continue;
			}

			if (term->mOperator == '/') {
				auto before_term = *(--res.end());
				res.pop_back();
				auto after_term = terms[i+1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(before_term);
				bundle->mTerms.push_back(term);
				bundle->mTerms.push_back(after_term);
				res.push_back(bundle);
				i++;
				continue;
			}
		}
		res.push_back(term);
	}
	
	return res;
}
vector<Term*> Solver::AddNonEssientalOps(vector<Term*> terms) {
	vector<Term*> res;

	for (int i = 0; i < terms.size(); i++) {
		auto term = terms[i];
		if (i == 0) {
			res.push_back(term);
			continue;
		}
		if (term->mType == TermTypes::Bund) {
			auto bundle = static_cast<Bundle*>(term);
			for (auto *_term : bundle->mTerms)
				res.push_back(_term);
			continue;
		}

		if (term->mType == TermTypes::Brack) {
			res.push_back(term);
			continue;
		}
		
		// check if term was positive
		if (term->mValue > 0) {
			res.push_back(new Operator('+'));
			res.push_back(term);
			continue;
		}
		else if (term->mValue < 0) {
			term->mValue = abs(term->mValue);
			res.push_back(new Operator('-'));
			res.push_back(term);
			continue;
		}
	}

	return res;
}

Bundle * Solver::BuildBundle(vector<Term*> terms) {
	Bundle* bundle = new Bundle();
	for (int i = 0; i < terms.size(); i++)
		bundle->mTerms.push_back(terms[i]);
	return bundle;
}
vector<Term*> Solver::BreakBundle(Bundle * bundle) {
	vector<Term*> res;
	for (auto *item : bundle->mTerms)
		res.push_back(item);
	return res;
}

vector<Term*> Solver::BundleTermsDuo(vector<Term*> terms) {
	vector<Term*> res;

	for (int i = 0; i < terms.size(); i++) {
		auto term = terms[i];
		if (term->mType == TermTypes::Op) {
			if (term->mOperator == '/') {
				if (i == 0) {
					// invalid cant have division at begining
					cout << "Can't have division at begining!" << endl;
					system("PAUSE");
					exit(0);
				}
				auto before_term = terms[i-1];
				auto after_term = terms[i +1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(before_term);	// t1
				bundle->mTerms.push_back(term);			// op
				bundle->mTerms.push_back(after_term);	// t2
				res.push_back(bundle);
				i++;
				continue;
			}
			else {
				// if it was a multiplication
				if (term->mOperator == '*') {
					if (i == 0) {
						// invalid cant have multiplication at begining
						cout << "Can't have multiplication at begining!" << endl;
						system("PAUSE");
						exit(0);
					}
					auto before_term = terms[i-1];
					auto after_term = terms[i + 1];
					Bundle* bundle = new Bundle();
					bundle->mTerms.push_back(before_term);	// t1
					bundle->mTerms.push_back(term);			// op
					bundle->mTerms.push_back(after_term);	// t2
					res.push_back(bundle);
					i++;
					continue;
				}

				// if it was not division and not multiplication
				auto after_term = terms[i + 1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(term);		  // operator
				bundle->mTerms.push_back(after_term); // term
				res.push_back(bundle);
				i++;
				continue;
			}
		}
		res.push_back(term);
	}

	return res;
}
vector<Term*> Solver::BundleTermsTri(vector<Term*> terms) {
	vector<Term*> res;

	for (int i = 0; i < terms.size(); i++) {
		auto term = terms[i];
		if (term->mType == TermTypes::Op) {
			if (term->mOperator == '/') {
				if (i == 0) {
					// invalid cant have division at begining
					cout << "Can't have division at begining!" << endl;
					system("PUASE");
					exit(0);
				}
				auto before_term = terms[i - 1];
				auto after_term = terms[i + 1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(before_term); // t1
				bundle->mTerms.push_back(term);		   // op
				bundle->mTerms.push_back(after_term);  // t2
				res.push_back(bundle);
				i++;
				continue;
			}
			else {
				// if it was a multiplication
				if (term->mOperator == '*') {
					if (i == 0) {
						// invalid cant habe multiplication at begining
						cout << "Can't have multiplication at beginign!" << endl;
						system("PAUSE");
						exit(0);
					}
					auto before_term = terms[i - 1];
					auto after_term = terms[i + 1];
					Bundle* bundle = new Bundle();
					bundle->mTerms.push_back(before_term);	// t1
					bundle->mTerms.push_back(term);			// op
					bundle->mTerms.push_back(after_term);	// t2
					res.push_back(bundle);
					i++;
					continue;
				}

				// if it was not division and not multiplication
				auto before_term = terms[i - 1];
				auto after_term = terms[i + 1];
				Bundle* bundle = new Bundle();
				bundle->mTerms.push_back(before_term);
				bundle->mTerms.push_back(term);
				bundle->mTerms.push_back(after_term);
				res.push_back(bundle);
				i++;
				continue;
			}
		}
		res.push_back(term);
	}

	return res;
}

bool Solver::IsEqualBundle(Bundle * b1, Bundle * b2) {
	auto b1_copy = OrderTermsByValue(b1->mTerms);
	auto b2_copy = OrderTermsByValue(b2->mTerms);

	if (b1_copy.size() != b2_copy.size()) return false;

	for (int i = 0; i < b1_copy.size(); i++)
		if (!QMEvalHelper::IsEquTerms(b1_copy[i], b2_copy[i])) 
			return false;

	return true;
}

vector<Term*> Solver::OrderTermsByValue(vector<Term*> terms) {
	vector<Term*> res;
	res = RemNonEssientalOps(terms);
	stable_sort(res.begin(), res.end(), QMEvalHelper::IsHigherSig);
	res = AddNonEssientalOps(res);
	return res;
}
