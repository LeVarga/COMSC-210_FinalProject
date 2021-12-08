#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <ctime>
#include "Concession.h"
using namespace std;

Item Item::parse(const string& line) {
  istringstream istr(line);
  string fields[2];
  string tmp;
  int i = 0;
  while (getline(istr, tmp, '$')) {
    fields[i++] = tmp;
  }
  Item item;
  item.name = fields[0];
  item.cost = atof(fields[1].c_str());
  return item;
}

Vendor Vendor::parse(const string& line)
{
  istringstream istr(line);
  vector<string> fields;
  string tmp;
  while (getline(istr, tmp, ';')) {
    fields.push_back(tmp);
  }
  Vendor vendor;
  vendor.name = fields[0];
  vendor.location = fields[1];
  for (int i = 2; i < fields.size(); ++i) {
    vendor.items.push_back(Item::parse(fields[i]));
  }
  return vendor;
}

void ConcessionsController::loadConcessions(const string& filename)
{
  ifstream File(filename);
  string buf;
  while (getline(File, buf))
  {
    vendors.push_back(Vendor::parse(buf));
  }
}

vector<Vendor> ConcessionsController::getVendors() const
{
  return vendors;
}

string ConcessionsController::concessionConfCode()
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
