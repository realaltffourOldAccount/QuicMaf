#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <deque>
#include <stack>

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

#define DEF_C -999
#define DEF_N 0.0
typedef long double NValue;
typedef char CValue;

enum TermTypes {
	None,
	Const,
	Var,
	Op,
	Brack,
	Equ,
};


// for string delimiter
static vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}


static void removeSubstrs(string& s, string& p) {
	string::size_type n = p.length();
	for (string::size_type i = s.find(p);
		i != string::npos;
		i = s.find(p))
		s.erase(i, n);
}

int getPrecedence(char c) {
	//if (!isArithmitic(c)) return -1; // invalid

	if (c == '*') return 1;
	if (c == '/') return 1;
	if (c == '+') return 0;
	if (c == '-') return 0;
	return -1;
}


// return true if c1 is higher precedence
// return false if c2 is higher precedence
bool compPrecedence(char c1, char c2) {
	return getPrecedence(c1) > getPrecedence(c2);
}

static bool isArithmitic(char c) {
	if (c == '-') return true;
	if (c == '+') return true;
	if (c == '/') return true;
	if (c == '*') return true;
	return false;
}

static bool isEqualChar(char c) {
	if (c == '=') return true;
	return false;
}
static bool isPower(char c) {
	if (c == '^') return true;
	return false;
}
static bool isBrackets(char c) {
	if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c== '}') return true;
	return false;
}

static bool isBracketsOpening(char c) {
	if (c == '(' || c == '[' || c == '{') return true;
	return false;
}

static bool isBracketsClosing(char c) {
	if (c == ')' || c == ']' || c == '}') return true;
	return false;
}

static bool isContain(string str, char c) {
	for (int i = 0; i < str.size(); i++) if (str[i] == c) return true;
	return false;
}

static bool isContainAlph(string str) {
	for (int i = 0; i < str.size(); i++)
		if (isalpha(str[i])) return true;
	return false;
}

static bool isContainDigit(string str) {
	for (int i = 0; i < str.size(); i++)
		if (isdigit(str[i])) return true;
	return false;
}

static int indexofAlph(string str) {
	if (!isContainAlph(str)) return -1;

	for (int i = 0; i < str.size(); i++)
		if (isalpha(str[i]))
			return i;

	return-1;
}

static int indexofChar(string str, char c) {
	for (int i = 0; i < str.size(); i++)
		if (str[i] == c) return i;
	return -1;
}


static bool is_all_digits(const std::string &str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

static int getTermType(string str) {
	if (isContain(str, '=')) return Equ;
	if (isContain(str, '+')) return Op;
	if (isContain(str, '-')) return Op;
	if (isContain(str, '/')) return Op;
	if (isContain(str, '*')) return Op;

	if (is_all_digits(str)) return Const;
	else return Var;
}

#endif // !
