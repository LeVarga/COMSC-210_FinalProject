#ifndef CONCESSION_H
#define CONCESSION_H

#include <string>
#include <vector>
#include "Queue.h"
#include "DynamicArray.h"
using namespace std;

struct Item {
    string name;
    double cost;
    static Item parse(const string&);
};

struct Vendor {
    string name;
    string location;
    vector<Item> items;
    static Vendor parse(const string&);
};

class ConcessionsController {
    vector<Vendor> vendors;
public:
    void loadConcessions(const string&);
    vector<Vendor> getVendors() const;
    string concessionConfCode();
};

#endif