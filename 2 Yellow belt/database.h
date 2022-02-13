#pragma once

#include "date.h"
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Database {
public:
    void Add(const Date& date, const string& event);

    void Print(ostream& os) const;

    int RemoveIf(function<bool(const Date&, const string& event)> predicate);

    vector<string> FindIf(function<bool(const Date&, const string& event)> predicate) const;

    // throws invalid_argument if there is no last event for the given date
    string Last(const Date& date) const;

private:
    map<Date, vector<string>> storage;
    set<pair<Date, string>> uniqueEvents;
};

// Tests
void TestDatabaseAdd();

void TestDatabaseRemoveIf();

void TestDatabaseFindIf();

void TestDatabaseLast();
