#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class History
{
public:
    bool saveHistory(string);
	int getLinesCount();
	vector<string> getHistory();
	string getUserName();
};

#endif // HISTORY_H
