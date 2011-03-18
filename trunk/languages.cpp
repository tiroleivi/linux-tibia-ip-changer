#include "languages.h"
#include "history.h"

vector<string> Languages::getLanguagesFiles(){
	History h;
	vector<string>tab;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(string("/home/" + h.getUserName() +"/.linux-ip-changer/Languages").c_str())) == NULL) {
        cout << "Error opening " << "/home/"<< h.getUserName() <<"/.linux-ip-changer/Languages" << endl;
        return tab;
    }

    while((dirp = readdir(dp)) != NULL){
		string s = string(dirp->d_name);
		if(s.substr(0,1) != "."){
			if(s.find(".lang"))
				tab.push_back(s);
		}
    }
    closedir(dp);
    count = int(tab.size());
    return tab;
}

string Languages::loadLanguage(string lname){
	History h;
	vector<string>name;
	name = getLanguagesFiles();
	for(int i = 0; i < name.size(); i++){
		ifstream file(string("/home/" + h.getUserName() +"/.linux-ip-changer/Languages/" + name.at(i)).c_str());
		if(!file.is_open())
			return "";
		
		string line, tmp;
		bool b = false;
		while(getline(file, line)){
			if(line.substr(0,1) != "#"){
				if(lname == line)
					b = true;
				tmp += line + ",";
			}
		}
		if(b)
			return tmp;
	}
}

vector<string> Languages::getLanguagesList(){
	History h;
	vector<string>name, tab;
	name = getLanguagesFiles();
	for(int i = 0; i < name.size(); i++){
		ifstream file(string("/home/" + h.getUserName() +"/.linux-ip-changer/Languages/" + name.at(i)).c_str());
		if(!file.is_open())
			return tab;
		
		string line;
		while(getline(file, line)){
			if(line.substr(0,1) != "#"){
				tab.push_back(line);
				break;
			}
		}
	}
	return tab;
}
