#include "setup.h"
#include "history.h"

bool Setup::saveSetup(string v, string value){
	History h;
	ifstream file(string("/home/" + h.getUserName() +"/.linux-ip-changer/config.cfg").c_str());
    if(!file.is_open())
		return false;

	bool b = true;
    string line; 
    ofstream plik(string("/home/" + h.getUserName() +"/.linux-ip-changer/config.cfg").c_str()); 
    while(getline(file, line)){
		if(line.find(v)!=string::npos)
			plik<<line<<endl;
		else{
			plik<<v<<"=\""<<value<<"\""<<endl;
			b = false;
		}
    }
    if(b)
		plik<<v<<"=\""<<value<<"\""<<endl;

	plik.close();
    return true;
}

string Setup::loadSetupValue(string v){
	History h;
	ifstream file(string("/home/" + h.getUserName() +"/.linux-ip-changer/config.cfg").c_str());
    if(!file.is_open())
		return "";
	
    string line;
    while(getline(file, line)){
		if(line.find(v)!=string::npos){
			string tmp;
			bool b = false;
			for(int i = 0; i < line.size(); i++){
				string s;
				s += line.at(i);
				if(s == "\"" || s == "'")
					b = !b;
				
				if(b && !(s == "\"" || s == "'"))
					tmp += s;
				
				s.clear();
			}
			if(tmp != "")
				return tmp;
		}
    }
    return "";
}
