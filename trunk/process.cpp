#include "process.h"

Process::Process(int PID) :
        pid(PID)
{}

bool Process::readMemory(unsigned addr, char *data, unsigned size){
    if(ptrace(PTRACE_ATTACH, pid, 0, 0)!=0)
        return false;

    wait(0);

    for(unsigned i=0;i<size;i+=sizeof(int)){
        int buff;
        buff=ptrace(PTRACE_PEEKDATA, pid, addr+i, 0);
        memcpy(data+i, &buff, sizeof(int));
    }

    if(ptrace(PTRACE_DETACH, pid, 0, 0)!=0)
        return false;

    return true;
}

bool Process::writeMemory(unsigned addr, const char *data, unsigned size){
    if(ptrace(PTRACE_ATTACH, pid, 0, 0)!=0)
        return false;
    wait(0);

    for(unsigned i=0;i<size;i+=sizeof(int)){
        int buff;
        memcpy(&buff, data+i, sizeof(int));
        ptrace(PTRACE_POKEDATA, pid, addr+i, buff);
    }

    if(ptrace(PTRACE_DETACH, pid, 0, 0)!=0)
        return false;

    return true;
}

std::string Process::readClientVersion(unsigned addr){
	char data[4];
	bool ret = readMemory(addr, data, 4);
	if(!ret)
		return "";
	std::string ver = data;
	ver = ver.substr(0, 3);
		
	return ver;
}
