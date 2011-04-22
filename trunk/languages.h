#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

class Languages
{
public:
	vector<string> getLanguagesFiles();
	string loadLanguage(string);
	vector<string> getLanguagesList();
	int count;
};

#endif //LANGUAGES_H
