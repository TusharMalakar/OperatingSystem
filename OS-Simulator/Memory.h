/*
  Title          : Memory.h
  Author         : Tushar Malakar
  Created on     : 20th November, 2018
  Description    : interface for the "PCB" class
  Purpose        : Class project
*/
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <iostream>
#include <sstream>
#include <iterator>
#include <string>

#include "Queues.h"
#include "PCB.h"
using namespace std;

/*
Contiguous Memory Allocation::

This program simulates contiguous memory management with “best fit” approach.
Contiguous Memory Allocation

The main memory must accommodate both the operating system and the
various user processes.We therefore need to allocate main memory in themost
efficient way possible. This section explains one early method, contiguous
memory allocation.
The memory is usually divided into two partitions: one for the resident
operating system and one for the user processes. We can place the operating
system in either low memory or high memory. The major factor affecting this
decision is the location of the interrupt vector. Since the interrupt vector is
often in low memory, programmers usually place the operating system in low
memory as well. Thus, in this text, we discuss only the situation in which
the operating system resides in low memory. The development of the other
situation is similar.
We usually want several user processes to reside in memory at the same
time. We therefore need to consider how to allocate available memory to the
processes that are in the input queue waiting to be brought into memory. In
contiguous memory allocation, each process is contained in a single section of
memory that is contiguous to the section containing the next process.


*/
// MemFrag is a memory fragmentation in the memory tracker.
// It can be attached to a process, or be a hole.

struct MemFrag{

    unsigned int byte_;
    PCB* owner_;
    bool hole_;

    // Fragmentation with an l_value reference
    MemFrag(const unsigned int &byte, PCB *owner, bool hole)
            : byte_{byte}, owner_{owner}, hole_{hole}{}

    // Fragmentation with an r_value reference
    MemFrag(const unsigned int &&byte, PCB *owner, bool hole)
            : byte_{move(byte)}, owner_{owner}, hole_{hole}{}
};

class Memory{
public:
    // Default constructor.
    Memory() = default;

    // Constructor; Erases the node from memory.
    ~Memory(){
        for (auto it = memory_tracker_.begin(); it != memory_tracker_.end(); ++it){
           memory_tracker_.erase(it);
        }
     }

    // Sets up the initial memory after user input.
    // param mem: The user initiated memory.
    void setupMemory(const unsigned int &mem){
       MemFrag* initial = new MemFrag{mem, nullptr, true};
       memory_tracker_.push_back(initial);
    }

    // Allocates the memory needed for a process.
    // param owner: The pointer to the process in memory being allocated.
    // return: True if memory allocated successfully. Otherwise false.
    bool allocate(PCB* owner){

       unsigned int byte = owner->getMemorySize();
       for(auto it = memory_tracker_.begin(); it != memory_tracker_.end(); ++it){
        if(((*it)->hole_ == true && (*it)->byte_ >= byte)){
            MemFrag* memory_node = new MemFrag(byte, owner, false);
            cout << "Allocating memory...\n";
            memory_tracker_.insert(it, memory_node);
            auto it2 = it++;
            (*it2)->byte_ -= byte;
            //tail hole node has no memory left
            if((*it2)->byte_ == 0){
                memory_tracker_.pop_back();
            }
            return true;
        }
      }
      //no memory available
      return false;
    }


    // Deallocates the memory for a process.
    // param owner: The pointer to the process in memory being deallocated.
    void deallocate(PCB* owner){
      for(auto it = memory_tracker_.begin(); it != memory_tracker_.end(); ++it){

        if((*it)->owner_ == owner){

            (*it)->owner_ = nullptr;
            (*it)->hole_ = true;

            break;
        }
      }
     delete owner;
     owner = nullptr;
     merge();
   }

    // Gets a snapshot of the current memory.
    // return: A string of the memory snapshot.
    string printMemory(){
       stringstream ss;
       //range tracker
       unsigned int start = 0;
       auto it = memory_tracker_.end();
       it--;
       ss << " ===================================\n";
       ss << "|       Random Access Memory        |\n";
       ss << " ===================================\n\n";
       for (auto &node: memory_tracker_){
        ss << " -----------------------------------\n";
        ss << "\t" << start << " -> " << start+node->byte_-1 << endl;
        ss << " -----------------------------------\n\n";
        if (node->hole_ == true){
            ss << "\tEMPTY\n";
            start += node->byte_;
        } else {
            ss << node->owner_->getProcessInfo();
            start += node->byte_;
        }
       }
       return ss.str();
    }
private:
    // A list of MemFrag pointers where each node is a segment of memory.
    // Could be filled with a process or empty.
    list<MemFrag*> memory_tracker_;

    // Merges any adjacent holes in the memory after a deallocation.
    void merge(){
       list<MemFrag*>::iterator it2;
       for(auto it = memory_tracker_.begin(); it != prev(memory_tracker_.end()); ++it){
        it2 = it;
        it2++;
        //adjacent holes
        if(((*it)->hole_ == true) && ((*it2)->hole_ == true)){
            (*it)->byte_ += (*it2)->byte_;
            memory_tracker_.erase(it2);
            it--;
        }
       }
    }
};




class System{

public:
 // Constructor and Destructor
 // CPU is set to nullptr on start up as nothing in running in the CPU.
 System():cpu_(nullptr){};
 ~System() = default;

 // Gets the process that is using the CPU.
 // return: A pointer of the process.
 PCB* getCPUprocess(){
                 return cpu_;
 }

 void setupMemory(const unsigned int &mem_size){
     mem_size_ = mem_size;
     ram_.setupMemory(mem_size_);
 }

void setDisk(const int &num_disks){
     //By default I have 5 IO devices
     const int &num_IO_ = 5;
     num_disks_ = num_disks;
     num_IO = num_IO_;
     queue_manager_.setupQueues(num_disks_, num_IO);
}

void setCPUProcess(PCB* process){
    if (process == nullptr){
        cpu_ = nullptr;
        return;
    }
    cpu_ = process;
    process->setState("Running");
    process->setLock(true);
}

void newProcess(string &command, int &pid_tracker){
    int priority = command[2]-'0';
    unsigned int mem_needed = stoul(command.substr(4));

    if(priority < 0 || priority > 4){
        cout << "ERROR: Invalid priority level. < 1-4 >\n";
        return;
    }

    if(mem_needed <= 0){
        cout << "ERROR: Invalid memory size.\n";
        return;
    }

    PCB* new_process = new PCB(pid_tracker, priority, mem_needed);
    if(!ram_.allocate(new_process)){
        cout << "ERROR: No memory available for process.\n";
        return;
    }
    pid_tracker++;

    checkCPU(new_process);
    cout << "Adding new process...\n";
}

void terminate(){
    if(cpu_ == nullptr){
        cout << "ERROR: Nothing running in CPU!\n";
        return;
    }
    cout << "Terminating current process...\n";

    ram_.deallocate(cpu_);

    getNextProcess();
    return;
}

void requestIO(string &command){
    int device_num = command[2]-'0';

    //checking if theres a valid process in cpu
    if(cpu_ == nullptr){
        cout << "ERROR: No process running in CPU!\n";
        return;
    }
    if(command[0] == 'p'){
        if(device_num >= num_IO){
            cout << "ERROR: I/O devices does not exist!\n";
            return;
        }
        queue_manager_.addToIO_Queue(device_num,cpu_);
        cout << "Requesting IO number " << device_num << "...\n";
    } else if (command[0] == 'd'){
        if(device_num >= num_disks_){
            cout << "ERROR: Disk does not exist!\n";
            return;
        }
        queue_manager_.addToDiskQueue(device_num,cpu_);
        cout << "Requesting disk number " << device_num << "...\n";
    }
    getNextProcess();
}


void interruptDisk(string &command){
    int device_num = command[2]-'0';

    if(device_num >= num_disks_){
        cout << "ERROR: Disk does not exist!\n";
        return;
    }
    if (queue_manager_.isDiskEmpty(device_num)){
        cout << "ERROR: Nothing in disk " << device_num << "!\n";
        return;
    }
    PCB* process_ending = queue_manager_.getDiskProcess(device_num);

    queue_manager_.removeFromDiskQueue(device_num);
    checkCPU(process_ending);
    cout << "Interrupting disk number " << device_num << "...\n";
}

void snapshot(string &command){
    if(command[2] == 'r'){
        cout << printReadyQueue();
        return;
    } else if (command[2] == 'i'){
        cout << queue_manager_.printIO_Queue();
        return;
    } else if (command[2] == 'm'){
        cout << ram_.printMemory();
        return;
    }
    cout << "ERROR: Invalid screenshot option.\n";
}

string printReadyQueue(){
    stringstream ss;
    ss << " ===================================\n";
    ss << "|              C P U                |\n";
    ss << " ===================================\n\n";
    ss << (cpu_ == nullptr ? "\tNo Process Running\n" : cpu_->getProcessInfo()) <<endl << endl;

    ss << " ===================================\n";
    ss << "|       R E A D Y  Q U E U E        |\n";
    ss << " ===================================\n\n";
    ss << queue_manager_.printReadyQueue();
    return ss.str();
}

void getNextProcess(){
    PCB* next = queue_manager_.getNextProcess();
    if (next != nullptr){
        setCPUProcess(next);
        queue_manager_.removeFromReadyQueue(next->getPriority());
    }
    setCPUProcess(next);
}

//if cpu is empty, add the new process to cpu

//else check the prority of the new process, if new has
//higher priority, preempt the running process and add the
//new process.

void checkCPU(PCB* process){

    if (cpu_ == nullptr){
        setCPUProcess(process);
        return;
    } else if (process->getPriority() <= cpu_->getPriority()){
        queue_manager_.addToReadyQueue(process);
        return;
    } else if (process->getPriority() > cpu_->getPriority()){

        queue_manager_.addToReadyQueue(cpu_);
        setCPUProcess(process);

        return;
    }
}

private:
    // A pointer to a process that is currently using the CPU.
    PCB* cpu_;
    // The memory size of the system.
    unsigned int mem_size_;
    // The number of disks in the system.
    int num_disks_;
    // The number of IO in the system.
    int num_IO;
    // An instance of a queue manager.
    QueueManager queue_manager_;
    // An instance of RAM.
    Memory ram_;


};








#endif
