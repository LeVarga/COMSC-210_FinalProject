#ifndef MERCH_H
#define MERCH_H

#include <string>
#include <vector>
#include "Queue.h"
#include "DynamicArray.h"
using namespace std;

struct Merch {
	string team;
	string type;
	double cost;
	static Merch parse(const string&);
};

class MerchController {
	int numMerch = 0;
	DynamicArray<Merch> merchandise;
public:
	void loadMerch(const string&);
	vector<Merch*> getMerchByTeam(const string&);
	string merchConfCode();
};

#endif