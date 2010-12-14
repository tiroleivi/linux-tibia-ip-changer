#include <iostream>
#include <cstdlib>
#include "process.h"
#include "change.h"
#include "clientsversions.h"

Change::Change(int PID) :
        pid(PID)
{}

bool Change::changeIP(std::string IP, int port, std::string v){
    errno=0;
    Process process(pid);
    
	int LoginServersStart, ServerStep, PortStep, RSA, ServerCount;
	for(int i = 0; i < int(sizeof(clients)/sizeof(clients[1])); i++){
		if(clients[i][0] == v){
			std::istringstream ilss(clients[i][1]);
			ilss >> std::setbase(0) >> LoginServersStart;
			std::istringstream iss(clients[i][2]);
			iss >> std::setbase(0) >> ServerStep;
			std::istringstream ips(clients[i][3]);
			ips >> std::setbase(0) >> PortStep;
			std::istringstream irsa(clients[i][4]);
			irsa >> std::setbase(0) >> RSA;
			ServerCount = atoi(clients[i][5].c_str());
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

    return true;
}
