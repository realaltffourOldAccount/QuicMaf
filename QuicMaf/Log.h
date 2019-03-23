/*
 * File:   Log.h
 * Author: Alberto Lepe <dev@alepe.com>
 * Edited By: Ayham Mamoun <ayhamaboualfadl@gmail.com>
 *
 * Created on December 1, 2015, 6:00 PM
 */

#ifndef LOG_H
#define LOG_H
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class LOG {
public:
	vector<string>* qm_log = new vector<string>();
	LOG() {}
	LOG &operator<<(const string &msg) {
		qm_log->push_back(msg);
		return *this;
	}
private:
};

#endif  /* LOG_H */