/*
  Title          : Multiprogramming.h
  Author         : Tushar Malakar
  Created on     : 20th November, 2018
  Description    : interface for the "PCB" class
  Purpose        : Class project
*/
#ifndef __MULTI_PROGRAMMING_H__
#define __MULTI_PROGRAMMING_H__

#include <iostream>
#include <list>
#include <string>
#include <cstdlib>

#include "PCB.h"
using namespace std;

/*
Multiprogram::

One of the most important aspects of operating systems is the ability
to multiprogram. A single program cannot, in general, keep either the CPU
or the I/O devices busy at all times. Single users frequently have multiple
programs running.

Multiprogramming increases CPUutilization by organizing
jobs (code and data) so that the CPU always has one to execute.
The idea is as follows: The operating system keeps several jobs in memory
simultaneously.

Since, in general, main memory is too small to
accommodate all jobs, the jobs are kept initially on the disk in the job pool.
This pool consists of all processes residing on disk awaiting allocation of main
memory.

The set of jobs in memory can be a subset of the jobs kept in the job
pool. The operating system picks and begins to execute one of the jobs in
memory. Eventually, the job may have to wait for some task, such as an I/O
operation, to complete.
*/

class Multiprogramming{

public:
	Multiprogramming(){
                    idle = true;
	}

	bool isIdle(){
                    return idle;
	}

	//when new process comes, it will be add to cpu if
	//cpu is not busy or add to readyqueue it cpu is busy.
	void addToDisk(PCB p){
	    if(idle){
                                current = p;
                                idle = false;
	    }
	    else{
                                readyQueue.push_back(p);

	    }
	}

	void exitDisk(PCB &p){

                     current.timeqMutate(0);
	     p = current;

	     if(!readyQueue.empty()){

		current = readyQueue.front();
		readyQueue.pop_front();
		idle = false;
	     }
	     else{
		current.terminate();
		idle = true;
	     }
	}

	void print(const int &x){
                                cout << "PCB currently using Disk " << x << ": " << endl;
                                if(current.pidAccess() >= 0){

		     cout << "\t" << "PCB PID: " << current.pidAccess() << " | ";
		     cout << "File the PCB requested: " << current.getFilename() << endl;
                                }

                                cout << "PCB in Disk " << x << " Queue's: " << endl;
                                for(auto x : readyQueue){

		     cout << "\t" << "PCB PID: " << x.pidAccess() << " | ";
		     cout << "File the PCB requested: " << x.getFilename() << endl;;
                                }

                                cout << endl;
	}

private:
	PCB current;
	list <PCB> readyQueue;
	bool idle;

};

#endif
