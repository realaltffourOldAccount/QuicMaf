#include "QMReducer.h"

vector<Term*> QMReducer::Reduce() {
	return vector<Term*>();
}

bool QMReducer::IsSolvable(Term * t1, Operator * op, Term * t2) {
	// Constants check
	if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) return true;
	if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var &&
		op->mOperator == '*') return true;

	// Variables check
	if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var &&
		IsEquPower(t1, t2)) return true;
	if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var &&
		!IsEquPower(t1, t2)) return false;

	// Brackets checks
	// Avoid calling is solvable on bracket since they will break 
	// in evaluation process
	if (t1->mType == TermTypes::Brack && t2->mType == TermTypes::Brack &&
		op->mOperator == '*')  return true;
	if (t1->mType == TermTypes::Brack && t2->mType != TermTypes::Brack &&
		op->mOperator == '*') return true;
	if (t2->mType == TermTypes::Brack && t1->mType != TermTypes::Brack &&
		op->mOperator == '*') return true;
	if (t1->mType == TermTypes::Brack && t2->mType != TermTypes::Brack &&
		op->mOperator != '*') return true;
	if (t2->mType == TermTypes::Brack && t1->mType != TermTypes::Brack &&
		op->mOperator != '*') return true;

	return false; // End situation
}
vector<Term*> QMReducer::BreakBracket(Bracket * brack, bool order, Identifier_t order_type) {
	vector<Term*> res;

	if (brack->GetConstant() == nullptr) {
		for (int i = 0; i < brack->mTerms.size(); i++)
			res.push_back(brack->mTerms[i]);
	}

	for (int i = 0; i < brack->mTerms.size(); i++) {
		auto returns = Mul(brack->GetConstant(), brack->mTerms[i]);
		for (int j = 0; j < returns.size(); j++)
			res.push_back(returns[j]);
	}
	return res;
}

vector<Term*> QMReducer::OrderTerms(vector<Term*> terms, Identifier_t order) {
	stable_sort(terms.begin(), terms.end(), [this](Term* t1, Term* t2) {return IsHigherSig(t1, t2); });
	return terms;
}
vector<Term*> QMReducer::getRangeOfTerms(vector<Term*> terms, int begin, int end) {
	vector<Term*> res;
	for (int i = begin; i < end; i++) res.push_back(terms[i]);
	return res;
	return res;
}

Bracket * QMReducer::convertToBracket(Term * t1) {
	if (t1->mType != TermTypes::Brack) return false;
	return static_cast<Bracket*>(t1);
}
Variable * QMReducer::convertToVariable(Term * t1) {
	if (t1->mType != TermTypes::Var) return false;
	return static_cast<Variable*>(t1);
}
Constant * QMReducer::convertToConstant(Term * t1) {
	if (t1->mType != TermTypes::Const) return false;
	return static_cast<Constant*>(t1);
}

bool QMReducer::IsHigherSig(Term * t1, Term * t2) { // x^2 > x :  true
	if (t1->mType == TermTypes::Brack)
		return true;
	else if (t1->mType == TermTypes::Op)
		return false;
	else if (t2->mType == TermTypes::Op)
		return true;
	if (t1->mType == TermTypes::Var) {
		// its a var check for other term
		if (t2->mType == TermTypes::Brack) return false;

		if (t2->mType == TermTypes::Var) {
			// if power is equal
			if (IsEquPower(t1, t2)) {
				// check for value
				return t1->mValue >= t2->mValue;
			}
			// if t1 has higher or equal power 
			if (t1->mPower >= t2->mPower) return true;
			else return false; // t2 is higher
		}

		return true;
	}
	else if (t1->mType == TermTypes::Const) {
		// its a const check for other term
		if (t2->mType == TermTypes::Brack) return false;
		if (t2->mType == TermTypes::Var) return false;
		if (t2->mType == TermTypes::Const) {
			// if same type
			// check for power
			return t1->mPower >= t2->mPower;
		}
	}

	return false;
}

bool QMReducer::IsEquTerms(Term * t1, Term * t2) {
	if (!IsEquTypes(t1, t2)) return false;
	if (!IsEquPower(t1, t2)) return false;
	if (!IsEquValue(t1, t2)) return false;
	if (!IsEquVariable(t1, t2)) return false;
	return true;
}

bool QMReducer::IsEquTypes(Term * t1, Term * t2) {
	return t1->mType == t2->mType;
}
bool QMReducer::IsEquPower(Term * t1, Term * t2) {
	return t1->mPower == t2->mPower;
}
bool QMReducer::IsEquValue(Term * t1, Term * t2) {
	return t1->mValue == t2->mValue;
}
bool QMReducer::IsEquVariable(Term * t1, Term * t2) {
	return t1->mVariable == t2->mVariable;
}
bool QMReducer::IsBracket(Term * t1) {
	return t1->mType == TermTypes::Brack;
}

vector<Term*> QMReducer::ReducePower(Term * t1) {
	if (IsBracket(t1)) return {}; // Brackets powers aren't allowed

	if (t1->mType == TermTypes::Var) return { t1 };
	else
		t1->mValue = pow(t1->mValue, t1->mPower);

	return { t1 };
}

vector<Term*> QMReducer::Add(Term * t1, Term * t2, Identifier_t order) {
	vector<Term*> res;


	// check if one of the terms were brackets
	if (IsBracket(t1) || IsBracket(t2)) {
		// check which term is bracket
		if (IsBracket(t1) && !IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t1));
			auto brokenbrack2 = BreakBracket(convertToBracket(t2));
			for (int i = 0; i < brokenbrack2.size(); i++)
				brokenbrack.push_back(brokenbrack[i]);
			// TODO: evaluation system
			return brokenbrack;
		}
		if (IsBracket(t1) && !IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t1));
			brokenbrack.push_back(t2);
			// TODO: evaluation system
			return brokenbrack;
		}
		else if (!IsBracket(t1) && IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t2));
			brokenbrack.push_back(t1);
			// TODO: evaluation system
			return brokenbrack;
		}
	}
	else {
		// Check if it was solvable
		if (IsSolvable(t1, new Operator('+'), t2)) {
			// only two cases that it is solvable
			// const + const
			// var + var : no var difference
			if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) {
				auto t1_const = convertToConstant(t1);
				auto t2_const = convertToConstant(t2);
				t1_const = convertToConstant(ReducePower(t1_const)[0]);
				t2_const = convertToConstant(ReducePower(t2_const)[0]);

				Constant* Const = nullptr;
				Const = new Constant();
				Const->mValue = t1_const->mValue + t2->mValue;
				return { Const };
			}
			else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) {
				auto t1_var = convertToVariable(t1);
				auto t2_var = convertToVariable(t2);

				Variable* var = nullptr;
				var = new Variable();
				var->mPower = t1_var->mPower;
				var->mValue = t1_var->mValue + t2_var->mValue;
				if (var->mValue == 0) {
					Constant* Const = nullptr;
					Const->mPower = t1_var->mPower;
					Const->mValue = t1_var->mValue + t2_var->mValue;
					return { Const };
				}
				var->mVariable = t1_var->mVariable;
				return { var };
			}
		}
		else { // if it wasn't solvable
			// only 3 situations it is not solvable
			// these situations are treated the same
			// order them depending on arg 'order'
			res = { t1, t2 };
			res = OrderTerms(res, order);
			return res;
		}
	}
	return res;
}

vector<Term*> QMReducer::Sub(Term * t1, Term * t2, Identifier_t order) {
	vector<Term*> res;


	// check if one of the terms were brackets
	if (IsBracket(t1) || IsBracket(t2)) {
		// check which term is bracket
		if (IsBracket(t1) && IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t1));
			auto brokenbrack2 = BreakBracket(convertToBracket(t2));
			for (int i = 0; i < brokenbrack2.size(); i++)
				brokenbrack.push_back(brokenbrack[i]);
			// TODO: evaluation system
			return brokenbrack;
		}
		if (IsBracket(t1) && !IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t1));
			brokenbrack.push_back(t2);
			// TODO: evaluation system
			return brokenbrack;
		}
		else if (!IsBracket(t1) && IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t2));
			brokenbrack.push_back(t1);
			// TODO: evaluation system
			return brokenbrack;
		}
	}
	else {
		// Check if it was solvable
		if (IsSolvable(t1, new Operator('-'), t2)) {
			// only two cases that it is solvable
			// const + const
			// var + var : no var difference
			if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) {
				auto t1_const = convertToConstant(t1);
				auto t2_const = convertToConstant(t2);
				t1_const = convertToConstant(ReducePower(t1_const)[0]);
				t2_const = convertToConstant(ReducePower(t2_const)[0]);

				Constant* Const = nullptr;
				Const = new Constant();
				Const->mValue = t1_const->mValue - t2->mValue;
				return { Const };
			}
			else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) {
				auto t1_var = convertToVariable(t1);
				auto t2_var = convertToVariable(t2);

				Variable* var = nullptr;
				var = new Variable();
				var->mPower = t1_var->mPower;
				var->mValue = t1_var->mValue - t2_var->mValue;
				if (var->mValue == 0) {
					Constant* Const = new Constant();
					Const->mPower = t1_var->mPower;
					Const->mValue = t1_var->mValue - t2_var->mValue;
					return { Const };
				}
				var->mVariable = t1_var->mVariable;
				return { var };
			}
		}
		else { // if it wasn't solvable
			// only 3 situations it is not solvable
			// these situations are treated the same
			// order them depending on arg 'order'
			res = { t1, t2 };
			res = OrderTerms(res, order);
			return res;
		}
	}
	return res;
}

vector<Term*> QMReducer::Mul(Term * t1, Term * t2, Identifier_t order) {
	vector<Term*> res;

	// check if one of the terms are brackets
	if (IsBracket(t1) || IsBracket(t2)) {
		// only 3 situations
		// brack, brack : use foil
		// brack, var
		// brack, const

		if (IsBracket(t1) || IsBracket(t2)) {
			auto t1_brokenbrack = BreakBracket(convertToBracket(t1), true, order);
			auto t2_brokenbrack = BreakBracket(convertToBracket(t2), true, order);

			for (int i = 0; i < t1_brokenbrack.size(); i++) {
				for (int j = 0; j < t2_brokenbrack.size(); j++) {
					auto returns = Mul(t1_brokenbrack[i], t2_brokenbrack[j], order); // recurse
					for (int m = 0; m < returns.size(); m++) res.push_back(returns[m]);
				}
			}

			// TODO: Make evaluation system
			return res;
		}
		// if only one bracket is there
		if (IsBracket(t1) && !IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t1), true, order);

			// if t2 was a variable or a constant same method
			for (int i = 0; i < brokenbrack.size(); i++) {
				auto returns = Mul(brokenbrack[i], t2);
				for (int j = 0; j < returns.size(); j++) res.push_back(returns[j]);
			}
			return res;
		}
		if (!IsBracket(t1) && IsBracket(t2)) {
			auto brokenbrack = BreakBracket(convertToBracket(t2), true, order);

			// if t2 was a variable or a constant same method
			for (int i = 0; i < brokenbrack.size(); i++) {
				auto returns = Mul(brokenbrack[i], t1);
				for (int j = 0; j < returns.size(); j++) res.push_back(returns[j]);
			}
			return res;
		}


	}
	// niether are brackets
	else {
		// no need to check solvability
		if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) {
			auto t1_var = convertToVariable(t1);
			auto t2_var = convertToVariable(t2);

			Variable* var = nullptr;
			var = new Variable();
			var->mValue = t1_var->mValue * t2_var->mValue;
			var->mVariable = t1_var->mVariable;
			var->mPower = t1_var->mPower + t2_var->mPower;
			return { var };
		}
		else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Const) {
			auto t1_var = convertToVariable(t1);
			auto t2_const = convertToConstant(ReducePower(t2)[0]);

			Variable* var = nullptr;
			var = new Variable();
			var->mValue = t1_var->mValue * t2_const->mValue;
			var->mVariable = t1_var->mVariable;
			var->mPower = t1_var->mPower;
			return { var };
		}
		else if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Var) {
			auto t1_const = convertToConstant(t1);
			auto t2_var = convertToVariable(ReducePower(t2)[0]);

			Variable* var = nullptr;
			var = new Variable();
			var->mValue = t2_var->mValue * t1_const->mValue;
			var->mVariable = t2_var->mVariable;
			var->mPower = t2_var->mPower;
			return { var };
		}
		else if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) {
			Constant* t1_const = new Constant(), *t2_const = new Constant();
			t1_const = convertToConstant(ReducePower(t1)[0]);
			t2_const = convertToConstant(ReducePower(t2)[0]);

			Constant* Const = nullptr;
			Const = new Constant();
			Const->mValue = t1_const->mValue * t2_const->mValue;
			return { Const };
		}
	}
	return res;
}

bool QMReducer::IsDivSolvable(Term * t1, Term * t2) {
	if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) return true;
	if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) return true;

	return false;
}

bool QMReducer::IsDivSpecialCase(Term * t1, Term * t2) {
	if (t1->mType == TermTypes::Brack &&
		t2->mType != TermTypes::Brack) return true;

	if (t1->mType != TermTypes::Brack &&
		t2->mType == TermTypes::Brack) return true;

	if (t1->mType == TermTypes::Brack &&
		t2->mType == TermTypes::Brack) return true;

	if (t1->mType == TermTypes::Const &&
		t2->mType == TermTypes::Var) return true;

	return false;
}

vector<Term*> QMReducer::gcdofTerms(Term * t1, Term * t2) {

	if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) {
		auto t1_const = convertToConstant(ReducePower(t1)[0]);
		auto t2_const = convertToConstant(ReducePower(t2)[0]);

		Constant* Const = new Constant();
		Const->mValue = gcf(t1->mValue, t2->mValue);
		return { Const };
	}
	else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) {
		auto t1_var = convertToVariable(t1);
		auto t2_var = convertToVariable(t2);

		Variable* var = new Variable();
		var->mValue = gcf(t1_var->mValue, t2_var->mValue);
		var->mVariable = t1_var->mVariable;
		var->mPower = (t1_var->mPower > t2_var->mPower) ? t2_var->mPower : t1_var->mPower;

		return { var };
	}
	else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Const) {
		auto t1_var = convertToVariable(t1);
		auto t2_const = convertToConstant(t2);

		Constant* Const = new Constant();
		Const->mValue = gcf(t1_var->mValue, t2_const->mValue);
		return { Const };
	}
	else if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Var) {
		auto t1_const = convertToConstant(t1);
		auto t2_var = convertToVariable(t2);

		Constant* Const = new Constant();
		Const->mValue = gcf(t1_const->mValue, t2_var->mValue);
		return { Const };
	}

	return {};
}

vector<Term*> QMReducer::FactorizeTermsToBrack(vector<Term*> terms, vector<Term*> terms2) {
	vector<Term*> allTerms = terms;
	for (int i = 0; i < terms2.size(); i++)
		allTerms.push_back(terms2[i]);

	Term* HCF_all = new Term();
	*HCF_all = *allTerms[0];
	for (int i = 0; i < allTerms.size(); i++)
		*HCF_all = *gcdofTerms(HCF_all, allTerms[i])[0];

	// make first bracket
	Bracket* brack1 = new Bracket();
	brack1->setConstant(HCF_all);
	for (int i = 0; i < terms.size(); i++)
		brack1->mTerms.push_back(Div(terms[i], HCF_all)[0]);

	// make second bracket
	Bracket* brack2 = new Bracket();
	brack2->setConstant(HCF_all);
	for (int i = 0; i < terms2.size(); i++)
		brack2->mTerms.push_back(Div(terms[i], HCF_all)[0]);

	return { brack1, brack2 };
}

vector<Term*> QMReducer::FactorizeTermsToBrack(vector<Term*> allTerms) {
	Term* HCF_all = new Term();
	*HCF_all = *allTerms[0];
	for (int i = 0; i < allTerms.size(); i++)
		*HCF_all = *gcdofTerms(HCF_all, allTerms[i])[0];

	// make bracket
	Bracket* brack = new Bracket();
	brack->setConstant(HCF_all);
	for (int i = 0; i < allTerms.size(); i++)
		brack->mTerms.push_back(Div(allTerms[i], HCF_all)[0]);

	return { brack };
}

bool QMReducer::TermsMatch(vector<Term*> terms1, vector<Term*> terms2) {
	terms1 = OrderTerms(terms1, Identifier_t::_high_order_left);
	terms2 = OrderTerms(terms2, Identifier_t::_high_order_left);
	return terms1 == terms2;
}

// WARNING: Make sure that t1 and t2 are in the simplest form.
vector<Term*> QMReducer::Div(Term * t1, Term * t2, Identifier_t order) {


	// Check if division is solvable
	if (IsDivSolvable(t1, t2)) {
		// only two situations
		// var / var, pow_no_matter
		// const / const, pow_no_matter

		if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) {
			// var / var
			Term* res = new Term();
			res->mValue = t1->mValue / t2->mValue;
			res->mPower = t1->mPower - t2->mPower;
			if (res->mPower == 0.0) 
				// it is a constant
				res->mType = TermTypes::Const;
			else
			res->mVariable = t1->mVariable;
			return { res };
		}
		else if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) {
			// const / const
			t1 = convertToConstant(ReducePower(t1)[0]);
			t2 = convertToConstant(ReducePower(t2)[0]);

			Constant* Const = new Constant();
			Const->mValue = t1->mValue / t2->mValue;
			return { Const };
		}
	}
	// if it was a special case
	else if (IsDivSpecialCase(t1, t2)) { 
		// only three situations: 
		// mult term / mult term
		// one term const / one term var
		// multi terms / one term
		// one term / multi terms

		if (t1->mType == TermTypes::Brack && t2->mType == TermTypes::Brack) {
			// multi terms / multi term
			auto t1_brack = convertToBracket(t1);
			auto t2_brack = convertToBracket(t2);

			// check if taking out common factors makes common bracket terms
			auto fraction_nomin = convertToBracket(FactorizeTermsToBrack(t1_brack->mTerms)[0]);
			auto fraction_domin = convertToBracket(FactorizeTermsToBrack(t2_brack->mTerms)[0]);

			if (TermsMatch(fraction_nomin->mTerms, fraction_domin->mTerms))
				// common bracket terms matched
				// TIME FOR RECURSION!
				return Div(fraction_nomin->GetConstant(), fraction_domin->GetConstant());

			// check if taking out common factors make common bracket coeffiecient
			auto fraction = FactorizeTermsToBrack(t1_brack->mTerms, t2_brack->mTerms);
			// fraction[0] is nomin
			// fraction[1] is domin
			fraction_nomin = convertToBracket(fraction[0]);
			fraction_nomin->setConstant(nullptr); // Nullptr == 1
			fraction_domin = convertToBracket(fraction[1]);
			fraction_domin->setConstant(nullptr); // Nullptr == 1

			Fraction* frac = new Fraction();
			
			frac->mNomin = fraction_nomin->mTerms;
			frac->mDomin = fraction_domin->mTerms;
			return { frac };
		}
		else if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Var) {
			// const / var
			auto t1_const = convertToConstant(ReducePower(t1)[0]);
			auto t2_var = convertToVariable(t2);

			Variable* var = new Variable();
			var->mVariable = t2_var->mVariable;
			var->mValue = t1_const->mValue / t2_var->mValue;
			var->mPower = t2_var->mPower;

			return { var };
		}
		else if (t1->mType == TermTypes::Brack && t2->mType != TermTypes::Brack) {
			// multi terms / multi terms
			auto t1_brack = convertToBracket(t1);

			vector<Term*> result;
			for (int i = 0; i < t1_brack->mTerms.size(); i++) {
				if (t1_brack->mTerms[i]->mType == TermTypes::Op) {
					result.push_back(t1_brack->mTerms[i]);
					continue;
				}

				auto resultant = Div(t1_brack->mTerms[i], t2);
				for (auto *term : resultant) result.push_back(term);
			}

			return result;
		}
		else if (t1->mType != TermTypes::Brack && t2->mType == TermTypes::Brack) {
			// one term / multi terms
			auto t2_Brack = convertToBracket(t2);

			vector<Term*> result;
			for (int i = 0; i < t2_Brack->mTerms.size(); i++) {
				if (t2_Brack->mTerms[i]->mType == TermTypes::Op) {
					result.push_back(t2_Brack->mTerms[i]);
				}

				auto resultant = Div(t1, t2_Brack->mTerms[i]);
				for (auto *term : resultant) result.push_back(term);
			}

			return result;
		}
	}

	return {};
}
