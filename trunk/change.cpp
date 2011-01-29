#include <iostream>
#include <cstdlib>
#include "process.h"
#include "change.h"
#include "clientsversions.h"

Change::Change(int PID) :
        pid(PID)
{}

std::string lastIp;

bool Change::changeIP(std::string IP, int port, std::string v){
    errno=0;
    Process process(pid);
    if(lastIp.size() < 2)
		lastIp = "tib";
    
    bool ver = false;
	int LoginServersStart, ServerStep, PortStep, RSA, ServerCount;
	for(int i = 0; i < int(sizeof(clients)/sizeof(clients[1])); i++){
		if(v == "Auto")
			ver = isCorrectVersion(clients[i][0]);

		if(clients[i][0] == v || ver){
			std::istringstream ilss(clients[i][1]);
			ilss >> std::setbase(0) >> LoginServersStart;
			std::istringstream iss(clients[i][2]);
			iss >> std::setbase(0) >> ServerStep;
			std::istringstream ips(clients[i][3]);
			ips >> std::setbase(0) >> PortStep;
			std::istringstream irsa(clients[i][4]);
			irsa >> std::setbase(0) >> RSA;
			ServerCount = atoi(clients[i][5].c_str());
			if(!isCorrectVersion(clients[i][0]))
				return false;
			
			break;
		}
	}

    bool result=process.writeMemory(RSA, RSA_KEY, strlen(RSA_KEY)+1);
    if(!result||errno)
        return false;

    for(unsigned s=0;s<unsigned(ServerCount);s++){
        result=process.writeMemory(LoginServersStart+s*ServerStep,
                        IP.c_str(), IP.size()+1);

        if(!result||errno)
            return false;
    }

    for(unsigned s=0;s<unsigned(ServerCount);s++){
        result=process.writeMemory(LoginServersStart+s*ServerStep+PortStep,
                        (char*)&port, 4);
        if(!result||errno)
            return false;
    }

	lastIp = IP.substr(0, 3);
    return true;
}

bool Change::isCorrectVersion(std::string v){
	Process process(pid);
	int LoginServersStart;
	for(int i = 0; i < int(sizeof(clients)/sizeof(clients[1])); i++){
		if(clients[i][0] == v){
			std::istringstream ilss(clients[i][1]);
			ilss >> std::setbase(0) >> LoginServersStart;
			std::string s = process.readClientVersion(LoginServersStart);
			if(s == "tib" || s == "log" || s == lastIp)
				return true;
			else
				return false;
		}
	}
	
return false;
}
