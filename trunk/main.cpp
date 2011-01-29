#include <iostream>
#include <stdio.h>
#ifdef __GUI__
#include <wx/init.h> 
#include <wx/app.h>
#include "main.h"
#include "gui.h"
#include "history.h"
#endif
#include "change.h"
#include "tibiapid.h"
#include "clientsversions.h"

TibiaPid pid;
using namespace std;
void usage(string w){
	if(w == "h"){
		cout<<"This program change IP tibia client in order to connect to Open Tibia Server."<<endl<<"Continue Moraxus project."<<endl<<"License: GNU GPL"<<endl<<"Programmer: Miziak ,Adress Finder: Virtelio."<<endl;
		cout<<"Usage:"<<endl<<"     change -ip serverIp serverPort clientVersion                            This commond change IP"<<endl;
		cout<<"     change -un serverIp serverPort clientVersion unstandardTibiaProcessName This commond change IP"<<endl<<"     Client Versions:"<<endl;
		string str;
		for(int i = 0; i < int(sizeof(clients)/sizeof(clients[1])); i++)
			str += clients[i][0].insert(1, ".") + ", ";
		str = str.substr(0, str.size()-2);
		cout<<"     "<<str<<endl<<"     change -h This commond display this help page."<<endl;
	}
	else if(w == "v")
		cout<<"Version 1.5 dzeta"<<endl;
}

int main(int argc, char **argv)
{
#ifdef __GUI__
	History h;
	string history = "/home/" + h.getUserName() +"/.linux-ip-changer/history";
	string config = "/home/" + h.getUserName() +"/.linux-ip-changer/config.cfg";
#endif
	int agr = 1;
	while (argc > agr)
	{
		if(!strcmp(argv[1], "-ip")){
			agr = 4;
			--argc;
			++argv;
		} 
		else if(!strcmp(argv[1], "-un")){
			agr = 5;
			--argc;
			++argv;
		} 
		else if(!strcmp(argv[1], "-h")){
			usage("h");
			return 0;
		}
		else if(!strcmp(argv[1], "-v")){
			usage("v");
			return 0;
		}
		else{
#ifdef __GUI__
	wxApp::SetInstance(new MyApp());
	if(access(history.c_str(), F_OK) != 0){
		(void)system("mkdir ~/.linux-ip-changer");
		(void)system("touch ~/.linux-ip-changer/history");
	}
	if(access(config.c_str(), F_OK) != 0){
		(void)system("touch ~/.linux-ip-changer/config.cfg");
		ofstream plik(config.c_str()); 
		plik<<"lang=\"English\""<<endl; 
		plik.close();
	}
	return wxEntry(argc, argv);
#else
		usage("h");
		return 0;
#endif
		}
	}
	
	if (argc != agr || (argc == 1 && agr == 1)){
#ifdef __GUI__
	wxApp::SetInstance(new MyApp());
	if(access(history.c_str(), F_OK) != 0){
		(void)system("mkdir ~/.linux-ip-changer");
		(void)system("touch ~/.linux-ip-changer/history");
	}
	if(access(config.c_str(), F_OK) != 0){
		(void)system("touch ~/.linux-ip-changer/config.cfg");
		ofstream plik(config.c_str()); 
		plik<<"lang=\"English\""<<endl; 
		plik.close();
	}
	return wxEntry(argc, argv);
#else
		usage("h");
		return 0;
#endif
	}
	
	if(agr == 5)
		pid.name = argv[4];
	
	string v = argv[3], s ="";
	s += v.at(1);
	if(s == ".")
		v = v.erase(1,1);
	
	int tpid = pid.getTibiaPidConsole();
	if(tpid == 0){
		cout<<(argv[4]? argv[4] : "Tibia")<<" process not found!"<<endl;
		return 0;
	}

    Change c(tpid);
    bool ret = c.changeIP(argv[1], atoi(argv[2]), v);
    if(ret)
		cout<<"IP changed to "<<argv[1]<<":"<<atoi(argv[2])<<"!"<<endl;
	else
		cout<<"IP not changed!"<<endl;
    return ret;
}

#ifdef __GUI__
bool MyApp::OnInit()
{

    Panel *panel = new Panel(wxT("IP Changer"));
    panel->Show(true);

    return true;
}
#endif
