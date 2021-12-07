#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <ctime>
#include "Merch.h"
using namespace std;

Merch Merch::parse(const string& line)
{
	istringstream istr(line);
	string fields[3];
	string tmp;
	int i = 0;
	while (getline(istr, tmp, ';')) {
		fields[i++] = tmp;
	}
	Merch merchandise;
	merchandise.team = fields[0];
	merchandise.type = fields[1];
	merchandise.cost = atof(fields[2].c_str());
	return merchandise;
}

void MerchController::loadMerch(const string& filename)
{
	ifstream File(filename);
	string buf;
	while (getline(File, buf))
	{
		merchandise[numMerch] = Merch::parse(buf);
		numMerch++;
	}
}

vector<Merch*> MerchController::getMerchByTeam(const string& team)
{
	vector<Merch*> tempMerch;
	for (int i = 0; i < numMerch; i++)
	{
		if (merchandise[i].team == team) {
			tempMerch.push_back(&merchandise[i]);
		}
	}
	return tempMerch;
}

string MerchController::merchConfCode()
{
	srand(time(0));
	rand();
	const char alphanumBank[] = "1234567890" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
	string codeGen = "";
	for (int i = 0; i < 10; i++)
	{
		codeGen += alphanumBank[rand() % (sizeof(alphanumBank) - 1)];
	}
	return codeGen;
}
