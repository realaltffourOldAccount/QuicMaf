#include "QMReducerHelper.h"

vector<IdsWithTerms> QMEvalHelper::convertRangeToIds(vector<Term*> range) {
	vector<IdsWithTerms> result;

	for (int i = 0; i < range.size(); i++) {
		IdsWithTerms resultant;
		resultant._id = getIdenByTerm(range[i]);
		resultant._term = range[i];
		result.push_back(resultant);
	}

	return result;
}

bool QMEvalHelper::IsSolvable(Term * t1, Operator * op, Term * t2) {
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
vector<Term*> QMEvalHelper::BreakBracket(Bracket * brack, bool order, Identifier_t order_type) {
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

vector<Term*> QMEvalHelper::OrderTerms(vector<Term*> terms, Identifier_t order) {
	if (order == Identifier_t::_high_order_left)
		stable_sort(terms.begin(), terms.end(), [](Term* t1, Term* t2) {return IsHigherSig(t1, t2); });
	else stable_sort(terms.begin(), terms.end(), [](Term* t1, Term* t2) {return IsHigherSig(t2, t1); });;
	return terms;
}

vector<Term*> QMEvalHelper::getRangeOfTerms(vector<Term*> terms, int begin, int end) {
	vector<Term*> res;
	for (int i = begin; i <= end; i++) res.push_back(terms[i]);
	return res;
}
vector<Identifier_t*> QMEvalHelper::getRangeOfIden(vector<Identifier_t*> terms, int begin, int end) {
	vector<Identifier_t*> res;
	for (int i = begin; i <= end; i++) res.push_back(terms[i]);
	return res;
}

Bracket * QMEvalHelper::convertToBracket(Term * t1) {
	if (t1->mType != TermTypes::Brack) return false;
	return static_cast<Bracket*>(t1);
}
Variable * QMEvalHelper::convertToVariable(Term * t1) {
	if (t1->mType != TermTypes::Var) return false;
	return static_cast<Variable*>(t1);
}
Constant * QMEvalHelper::convertToConstant(Term * t1) {
	if (t1->mType != TermTypes::Const) return false;
	return static_cast<Constant*>(t1);
}

bool QMEvalHelper::IsHigherSig(Term * t1, Term * t2) { // x^2 > x :  true
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

bool QMEvalHelper::IsEquTerms(Term * t1, Term * t2) {
	if (!IsEquTypes(t1, t2)) return false;
	if (!IsEquPower(t1, t2)) return false;
	if (!IsEquValue(t1, t2)) return false;
	if (!IsEquVariable(t1, t2)) return false;
	return true;
}

bool QMEvalHelper::IsEquTypes(Term * t1, Term * t2) {
	return t1->mType == t2->mType;
}
bool QMEvalHelper::IsEquPower(Term * t1, Term * t2) {
	return t1->mPower == t2->mPower;
}
bool QMEvalHelper::IsEquValue(Term * t1, Term * t2) {
	return t1->mValue == t2->mValue;
}
bool QMEvalHelper::IsEquVariable(Term * t1, Term * t2) {
	return t1->mVariable == t2->mVariable;
}
bool QMEvalHelper::IsBracket(Term * t1) {
	return t1->mType == TermTypes::Brack;
}

vector<Term*> QMEvalHelper::ReducePower(Term * t1) {
	if (IsBracket(t1)) return {}; // Brackets powers aren't allowed

	if (t1->mType == TermTypes::Var) return { t1 };
	t1->mValue = pow(t1->mValue, t1->mPower);
	t1->mPower = 1;

	return { t1 };
}

vector<Term*> QMEvalHelper::Add(Term * t1, Term * t2, Identifier_t order) {
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
			res = { t1, new Operator('+') , t2 };
			return res;
		}
	}
	return res;
}

vector<Term*> QMEvalHelper::Sub(Term * t1, Term * t2, Identifier_t order) {
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

vector<Term*> QMEvalHelper::Mul(Term * t1, Term * t2, Identifier_t order) {
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

bool QMEvalHelper::IsDivSolvable(Term * t1, Term * t2) {
	if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Var) return true;
	if (t1->mType == TermTypes::Const && t2->mType == TermTypes::Const) return true;

	return false;
}
bool QMEvalHelper::IsDivSpecialCase(Term * t1, Term * t2) {
	if (t1->mType == TermTypes::Brack &&
		t2->mType != TermTypes::Brack) return true;

	if (t1->mType != TermTypes::Brack &&
		t2->mType == TermTypes::Brack) return true;

	if (t1->mType == TermTypes::Brack &&
		t2->mType == TermTypes::Brack) return true;

	if (t1->mType == TermTypes::Var &&
		t2->mType == TermTypes::Const) return true;

	if (t1->mType == TermTypes::Const &&
		t2->mType == TermTypes::Var) return true;

	return false;
}
vector<Term*> QMEvalHelper::gcdofTerms(Term * t1, Term * t2) {

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
vector<Term*> QMEvalHelper::FactorizeTermsToBrack(vector<Term*> terms, vector<Term*> terms2) {
	vector<Term*> allTerms;

	for (int i = 0; i < terms.size(); i++)
		if (terms[i]->mType != TermTypes::Op)
			allTerms.push_back(terms[i]);

	for (int i = 0; i < terms2.size(); i++)
		if (terms2[i]->mType != TermTypes::Op)
			allTerms.push_back(terms2[i]);

	Term* HCF_all = new Term();
	*HCF_all = *allTerms[0];
	for (int i = 0; i < allTerms.size(); i++)
		*HCF_all = *gcdofTerms(HCF_all, allTerms[i])[0];

	// make first bracket
	Bracket* brack1 = new Bracket();
	brack1->setConstant(HCF_all);
	for (int i = 0; i < terms.size(); i++)
		if (terms[i]->mType != TermTypes::Op)
			brack1->mTerms.push_back(Div(terms[i], HCF_all)[0]);
		else brack1->mTerms.push_back(terms[i]);

	// make second bracket
	Bracket* brack2 = new Bracket();
	brack2->setConstant(HCF_all);
	for (int i = 0; i < terms2.size(); i++)
		if (terms2[i]->mType != TermTypes::Op)
			brack2->mTerms.push_back(Div(terms2[i], HCF_all)[0]);
		else brack2->mTerms.push_back(terms2[i]);

	return { brack1, brack2 };
}
vector<Term*> QMEvalHelper::FactorizeTermsToBrack(vector<Term*> allTerms) {
	Term* HCF_all = new Term();
	*HCF_all = *allTerms[0];
	for (int i = 0; i < allTerms.size(); i++)
		if (allTerms[i]->mType != TermTypes::Op)
			*HCF_all = *gcdofTerms(HCF_all, allTerms[i])[0];

	// make bracket
	Bracket* brack = new Bracket();
	brack->setConstant(HCF_all);
	for (int i = 0; i < allTerms.size(); i++)
		if (allTerms[i]->mType != TermTypes::Op)
			brack->mTerms.push_back(Div(allTerms[i], HCF_all)[0]);
		else brack->mTerms.push_back(allTerms[i]);

	return { brack };
}
bool QMEvalHelper::TermsMatch(vector<Term*> terms1, vector<Term*> terms2) {
	terms1 = OrderTerms(terms1, Identifier_t::_high_order_left);
	terms2 = OrderTerms(terms2, Identifier_t::_high_order_left);

	if (terms1.size() != terms2.size()) return false;

	for (int i = 0; i < terms1.size(); i++)
		if (!IsEquTerms(terms1[i], terms2[i])) return false;

	return true;
}
// WARNING: Make sure that t1 and t2 are in the simplest form.
vector<Term*> QMEvalHelper::Div(Term * t1, Term * t2, Identifier_t order) {


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
		// only four situations: 
		// mult term / mult term
		// one term const / one term var
		// one term var / one term const
		// multi terms / one term
		// one term / multi terms

		if (t1->mType == TermTypes::Brack && t2->mType == TermTypes::Brack) {
			// multi terms / multi term
			auto t1_brack = convertToBracket(t1);
			auto t2_brack = convertToBracket(t2);

			// check if taking out highest common factors makes common bracket terms
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

			vector<Term*> res;
			for (int i = 0; i < fraction_nomin->mTerms.size(); i++)
				res.push_back(fraction_nomin->mTerms[i]);
			res.push_back(new Operator('/'));
			for (int i = 0; i < fraction_domin->mTerms.size(); i++)
				res.push_back(fraction_domin->mTerms[i]);

			return res;
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
		else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Const) {
			// var / const
			auto t1_var = convertToVariable(t1);
			auto t2_const = convertToConstant(ReducePower(t2)[0]);

			Variable* var = new Variable();
			var->mValue = t1_var->mValue / t2_const->mValue;
			var->mPower = t1_var->mPower;
			var->mVariable = t1_var->mVariable;

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

			auto fraction = FactorizeTermsToBrack({ t1 }, t2_Brack->mTerms);

			auto comfac = convertToBracket(fraction[0])->GetConstant();

			vector<Term*> res;
			for (auto *term : convertToBracket(fraction[0])->mTerms)
				res.push_back(term);
			res.push_back(new Operator('/'));
			for (auto *term : convertToBracket(fraction[1])->mTerms)
				res.push_back(term);

			return res;
		}
	}

	return {};
}
