#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#define DEF_C -999
#define DEF_N 0.0
typedef double NValue;
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
vector<string> split(string s, string delimiter) {
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


void removeSubstrs(string& s, string& p) {
	string::size_type n = p.length();
	for (string::size_type i = s.find(p);
		i != string::npos;
		i = s.find(p))
		s.erase(i, n);
}


bool isArithmitic(char c) {
	if (c == '-') return true;
	if (c == '+') return true;
	if (c == '/') return true;
	if (c == '*') return true;
	return false;
}

bool isEqualChar(char c) {
	if (c == '=') return true;
	return false;
}
bool isBrackets(char c) {
	if (c == '(') return true;
	return false;
}

bool isContain(string str, char c) {
	for (int i = 0; i < str.size(); i++) if (str[i] == c) return true;
	return false;
}

bool isContainAlph(string str) {
	for (int i = 0; i < str.size(); i++)
		if (isalpha(str[i])) return true;
	return false;
}

bool isContainDigit(string str) {
	for (int i = 0; i < str.size(); i++)
		if (isdigit(str[i])) return true;
	return false;
}

int indexofAlph(string str) {
	if (!isContainAlph(str)) return -1;

	for (int i = 0; i < str.size(); i++)
		if (isalpha(str[i]))
			return i;

	return-1;
}

int indexofChar(string str, char c) {
	for (int i = 0; i < str.size(); i++)
		if (str[i] == c) return i;
	return -1;
}


bool is_all_digits(const std::string &str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

int getTermType(string str) {
	if (isContain(str, '=')) return Equ;
	if (isContain(str, '+')) return Op;
	if (isContain(str, '-')) return Op;
	if (isContain(str, '/')) return Op;
	if (isContain(str, '*')) return Op;

	if (is_all_digits(str)) return Const;
	else return Var;
}

#endif // !
