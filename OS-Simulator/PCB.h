/*
  Title          : PCB.h
  Author         : Tushar Malakar
  Created on     : 20th November, 2018
  Description    : interface for the "Process" class
  Purpose        : Class project
*/
#ifndef __PCB_H__
#define __PCB_H__

/*
Process control block (PCB) ::
The process scheduling state: The state of the process in terms of "ready", "terminated", etc., and other scheduling information as well, like priority value, the
amount of time elapsed since the process gained control of the CPU or since it was suspended. Also, in case of a suspended process, event identification data must
be recorded for the event the process is waiting for.

Process structuring information: process's children id's, or the id's of other processes related to the current one in some functional way, which may be represented as a queue, a ring or other data structures.

Interprocess communication information: various flags, signals and messages associated with the communication among independent processes may be stored in the PCB.

Process Privileges in terms of allowed/disallowed access to system resources.

Process State: State may enter into new, ready, running, waiting, dead depending on CPU scheduling.

Process Number (PID): A unique identification number for each process in the operating system (also known as Process ID).

Program Counter (PC): A pointer to the address of the next instruction to be executed for this process.

CPU Registers: Indicates various register set of CPU where process need to be stored for execution for running state.

CPU Scheduling Information: indicates the information of a process with which it uses the CPU time through scheduling.

Memory Management Information: includes the information of page table, memory limits, Segment table depending on memory used by the operating system.

Accounting Information: Includes the amount of CPU used for process execution, time limits, execution ID etc.

I/O Status Information: Includes a list of I/O devices allocated to the process.
*/

#include <iostream>
#include <sstream>

using namespace std;

class PCB{
public:
    //zero parameter constructor
    PCB(): lock_(false){ state_ = "Ready"; }
    //destructor
    ~PCB() = default;
    //copy constructor
    PCB(const PCB &rhs) = default;
    //copy assignment
    PCB& operator=(const PCB &rhs) = default;
    //move constructor
    PCB(PCB && rhs) = default;
    //move assignment
    PCB& operator=(PCB &&rhs) = default;

    //three parameter constructor
    PCB(int &pid, int &priority, unsigned int &mem) :
    pid_{pid}, priority_{priority}, mem_size_{mem}{}
    PCB(int &&pid, int &&priority, unsigned int &&mem) :
    pid_{move(pid)}, priority_{move(priority)}, mem_size_{move(mem)}{}


    //setters
    void setLock(const bool &in_cpu){
                    lock_ = in_cpu;
    }
    void setState(const string &state){
                    state_ = state;
    }
    void setMemorySize(unsigned int &mem_size){
                    mem_size_ = mem_size;
    }

    //getters
    int getPID(){
                    return pid_;
    }
    bool getLockStatus(){
                    return lock_;
    }
    int getMemorySize(){
                    return mem_size_;
    }
    string getState(){
                    return state_;
    }
    int getPriority(){
                    return priority_;
    }

    string getProcessInfo(){

          stringstream ss;
          ss << "\tPID >> " << pid_ << endl << "\tPriority >> " << priority_ << endl
          << "\tProcess Size >> " << mem_size_ << endl << "\tState >> " << state_ << endl
          << "\tUsing CPU >> " << (lock_ == true ? "TRUE\n" : "FALSE\n");

         return ss.str();
}

private:
    int pid_;
    int priority_;
    unsigned int mem_size_;
    bool lock_;
    string state_;
}
;
#endif
