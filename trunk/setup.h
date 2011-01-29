#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

class Setup
{
public:
	bool saveSetup(string, string);
	string loadSetupValue(string);
};
