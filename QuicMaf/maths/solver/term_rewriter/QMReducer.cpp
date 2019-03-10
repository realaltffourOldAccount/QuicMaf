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
Variable * QMReducer::converToVariable(Term * t1) {
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
				auto t1_var = converToVariable(t1);
				auto t2_var = converToVariable(t2);

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
				auto t1_var = converToVariable(t1);
				auto t2_var = converToVariable(t2);

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
			auto t1_var = converToVariable(t1);
			auto t2_var = converToVariable(t2);

			Variable* var = nullptr;
			var = new Variable();
			var->mValue = t1_var->mValue * t2_var->mValue;
			var->mVariable = t1_var->mVariable;
			var->mPower = t1_var->mPower + t2_var->mPower;
			return { var };
		}
		else if (t1->mType == TermTypes::Var && t2->mType == TermTypes::Const) {
			auto t1_var = converToVariable(t1);
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
			auto t2_var = converToVariable(ReducePower(t2)[0]);

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
