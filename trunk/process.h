#ifndef PROCESS_H
#define PROCESS_H

#include <cstring>
#include <sys/ptrace.h>
#include <sys/wait.h>

class Process
{
    const unsigned int pid;
public:
    Process(int);
    bool readMemory(unsigned, char*, unsigned);
    bool writeMemory(unsigned, const char*, unsigned);
};

#endif // PROCESS_H
