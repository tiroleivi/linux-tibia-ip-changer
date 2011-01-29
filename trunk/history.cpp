#include "history.h"

string History::getUserName(){
	string data;
	char buffer[10];

	FILE *stream = popen("echo $USERNAME", "r");
	while(fgets(buffer, 10, stream) != NULL)
		data.append(buffer);
	pclose(stream);

	return data.substr(0, data.size()-1);
}

bool History::saveHistory(string new_host){
	ifstream file(string("/home/" + getUserName() +"/.linux-ip-changer/history").c_str());
    if(!file.is_open())
		return false;

	vector<string>out;
    string line; 
    while(getline(file, line)){
		if(line != new_host)
			out.push_back(line);
    }
    out.insert(out.begin(), new_host);

    ofstream plik(string("/home/" + getUserName() +"/.linux-ip-changer/history").c_str()); 
    for(int i = 0; i < int(out.size()); i++)
		plik<<out.at(i)<<endl; 
	plik.close();
    return true;
}

int History::getLinesCount(){
	int i = 0;
    ifstream file(string("/home/" + getUserName() +"/.linux-ip-changer/history").c_str());
    if(!file.is_open())
		return i;
	
	string line;
	while(getline(file, line))
		i++;
	return i;
}

vector<string> History::getHistory(){
	vector<string>tab;
	ifstream file(string("/home/" + getUserName() +"/.linux-ip-changer/history").c_str());
    if(!file.is_open())
		return tab;

    string line; 
    while(getline(file, line))
		tab.push_back(line);
	
    return tab;
}
