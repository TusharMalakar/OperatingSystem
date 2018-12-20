/*
  Title          : Queues.h
  Author         : Tushar Malakar
  Created on     : 20th November, 2018
  Description    : interface for the "PCB" class
  Purpose        : Class project
*/

#ifndef __CPU_H__
#define __CPU_H__

#include <iostream>
#include <unordered_map>
#include <list>
#include <sstream>

#include "PCB.h"

using namespace std;
/*
Preemptive CPU Scheduling using "priority queue"::

This class is implementing preemptive priority CPU-scheduling.
 Higher numbers mean higher priority.


CPU-scheduling decisions may take place under the following four circumstances:

1. When a process switches from the running state to the waiting state (for
example, as the result of an I/O request or an invocation of wait() for
the termination of a child process)

2.When a process switches from the running state to the ready state (for
example, when an interrupt occurs)

3. When a process switches from the waiting state to the ready state (for
example, at completion of I/O)

4. When a process terminates

*/


//First come first serve (FCFS)
class ReadyQueue{

public:
    // Default constructor/destructors.
    ReadyQueue() = default;
    ~ReadyQueue() = default;

    // Adds a process to the priority queue.
    // param process: The pointer of the process that is being added.
    void addProcess(PCB* process){
       readyQueue.push_back(process);
    }

    // Removes the first process from the priority queue.
    void removeProcess(){
        readyQueue.pop_front();
    }

    // Gets the first process from the priority queue.
    // return: The pointer to the first process in the queue.
    PCB* getProcess(){
        return readyQueue.front();
    }

    // Checks to see if the priority queue is empty.
    // True if empty. False otherwise.
    bool empty(){
        return readyQueue.empty();
    }

    // Shows the information of all the processes currently in the queue.
    // return: A string of the information.
    string showAllProcesses(){
           stringstream ss;

           if (readyQueue.empty()){
                ss << "\tReadyQueue is empty!\n";
                return ss.str();
           }

           for(auto &process: readyQueue){
                 ss << process->getProcessInfo() << endl;
           }
                return ss.str();
  }

 private:

    // A list of all the process pointers in the queue.
    list<PCB*> readyQueue;
};

//First come first serve (FCFS)
class IOQueue{

 public:
    // Adds a process to the IO queue.
    // param process: The pointer of the process to add to the IO queue.
    void addProcess(PCB* process){ ioQueue.push_back(process); }

    // Removes the first process from the IO queue.
    void removeProcess(){ ioQueue.pop_front(); }

    // Gets the first process from the IO queue.
    // return: A pointer to the first process in the queue.
    PCB* getProcess(){ return ioQueue.front(); }



    // Checks to see if the IO queue is empty.
    // True if empty. False otherwise.
    bool isEmpty(){ return ioQueue.empty();}

    // Shows the information of all the processes currently in the queue.
    // return: A string of the information.
    string showAllProcesses(){
        stringstream ss;

        if (ioQueue.empty()){

        ss << "\tQueue is empty!\n";
        return ss.str();
       }

       for(auto &process: ioQueue){
        if (process == ioQueue.front()){
            ss << "\tUSING DEVICE: \n\n";
            ss << process->getProcessInfo() << endl;
            ss << "\t---------------------------\n";
            if(ioQueue.size() == 1){
                ss <<"\t    No Process Waiting \n";
            } else {
                ss << "\t   Processes Waiting\n";
            }
            ss << "\t---------------------------\n";
        }
        else {
            ss << process->getProcessInfo() << endl;
        }
    }

    return ss.str();
}


private:
    //list of processes
    list<PCB*> ioQueue;
};

class QueueManager{
public:
    // constructor/destructor
    QueueManager(){
        ready_queue_.rehash(10);
        IO_queue.rehash(10);
        disk_queue_.rehash(10);
    }
    ~QueueManager() = default;

    // Sets up the ready queues and the IO queues.
    // param num_disks: the number of disks in the system.
    // param IO: the number of printers in the system.
    void setupQueues(const int &num_disks, const int & numIO){
	//creating ready queue
                    for(int priority_num = 0; priority_num <= 4; priority_num++){
                        ReadyQueue* new_priority = new ReadyQueue;
                        pair<int, ReadyQueue*> add_priority_queue (priority_num, new_priority);
                        ready_queue_.insert(add_priority_queue);
                    }
                    for (int device_num = 0; device_num < num_disks; device_num++){
                        IOQueue* new_io = new IOQueue;
                        pair<int, IOQueue*> add_io_queue (device_num, new_io);
                        disk_queue_.insert(add_io_queue);
                    }
                    for (int device_num = 0; device_num < numIO; device_num++){
                        IOQueue* new_io = new IOQueue;
                        pair<int, IOQueue*> add_io_queue (device_num, new_io);
                        IO_queue.insert(add_io_queue);
                    }
   }


    // Get the ready queue information.
    // return: A string of all the ready queue information.
    string printReadyQueue(){
                    stringstream ss;
                    for(auto &queue: ready_queue_){
                        ss << "Priority Queue: " << queue.first << endl << endl;
                        ss << queue.second->showAllProcesses() << endl;
                    }
                    return ss.str();
    }



    // Prints I/O queue information
    // return: A string of all the IO queues information.
    string printIO_Queue(){
                    stringstream ss;
                    ss << " ====================================\n";
                    ss << "|         D I S K  Q U E U E         |\n";
                    ss << " ====================================\n\n";
                    for(auto &queue: disk_queue_){
                        ss << "Disk Number: " << queue.first << endl << endl;
                        ss << queue.second->showAllProcesses() << endl;
                    }
                    return ss.str();
   }
    // Adds a process to the ready queue.
    // param process: Pointer to the process that is being added.
    void addToReadyQueue(PCB* process){
                    process->setLock(false);
                    process->setState("Ready");
                    getPriorityQueue(process->getPriority())->addProcess(process);
   }

    // Adds a process to the disk queue.
    // param device_num: The disk number queue that is being added to.
    // param process: The pointer of the process that is being added to the disk queue.
    void addToDiskQueue(const int &device_num, PCB* process){
                    process->setLock(false);
                    process->setState("Waiting");
                    getDiskQueue(device_num)->addProcess(process);
    }
    // Adds a process to the IO queue.
    // param device_num: The IO number queue that is being added to.
    // param process: The pointer of the process that is being added to the IO queue.
    void addToIO_Queue(const int &device_num, PCB* process){
                    process->setLock(false);
                    process->setState("Waiting");
                    getIO_queue(device_num)->addProcess(process);
   }

    // Removes the first process in the priority queue.
    // param priority: Priority of the priority queue to remove from.
    void removeFromReadyQueue(const int &priority){
                    getPriorityQueue(priority)->removeProcess();
    }

    // Removes a process from the disk queue.
    // param device_num: The disk number that the process is being removed from.
    void removeFromDiskQueue(const int &device_num){
                    getDiskQueue(device_num)->removeProcess();
    }

    // Removes a process from the IO queue.
    // param device_num: The IO number that the process is being removed from.
    void removeFromIO_Queue(const int &device_num){
                    getIO_queue(device_num)->removeProcess();
    }

    // Checks to see if the ready queue is empty
    // return: True if the queue is empty. False otherwise.
    bool isReadyEmpty(){
    for(auto & level: ready_queue_){
        if(!level.second->empty()){
            return false;
        }
    }
    return true;
}

    // Checks to see if the IO queue is empty.
    // param device_num: The number of the IO queue.
    // return: True if the queue is empty. False otherwise.
    bool IO_isEmpty(int &device_num){
                    return getIO_queue(device_num)->isEmpty();
    }

    // Checks to see if the disk queue is empty.
    // param device_num: The number of the disk queue.
    // return: True if the queue is empty. False otherwise.
    bool isDiskEmpty(int &device_num){ return getDiskQueue(
                                                           device_num)->isEmpty();
    }

    // Gets and sets the next process to use the CPU.
    // return: The process pointer of the next process in CPU.
    PCB* getNextProcess(){
                    for(auto &level: ready_queue_){
                        if(!level.second->empty()){
                            return level.second->getProcess();
                        }
                    }
                    //ready queue is empty
                    return nullptr;
    }

    // Gets the next process to use the IO.
    // param device_num: The number of the IO queue.
    // return: The process pointer of the next process in the queue.
    PCB* getIO_Process(int &device_num){
                    return getIO_queue(device_num)->getProcess();
    }

    // Gets the next process to use the disk.
    // param device_num: The number of the disk queue.
    // return: The process pointer of the next process in the queue.
    PCB* getDiskProcess(int &device_num){
                    return getDiskQueue(device_num)->getProcess();
    }

private:

    // The map of each priority queue in the Ready queue.
    // Each priority queue is mapped by their priority value.
    unordered_map<int, ReadyQueue*> ready_queue_;

    // The map of each disk queue.
    // Each disk queue is mapped to by their disk number.
    unordered_map<int, IOQueue*> disk_queue_;

    // The map of each IO queue.
    // Each disk queue is mapped to by their IO number.
    unordered_map<int, IOQueue*> IO_queue;



    // Gets the pointer to a specific IO queue.
    // param device_num: The number of the IO queue.
    // return: The pointer to the IO queue.
    IOQueue* getIO_queue(const int &device_num) {
                    return IO_queue.at(device_num);
    }

    // Gets the pointer to a specific disk queue.
    // param device_num: The number of the disk queue.
    // return: The pointer to the disk queue.
    IOQueue* getDiskQueue(const int &device_num) {
                    return disk_queue_.at(device_num);
    }

    // Gets the pointer to a specific priority queue.
    // param priority: The number of the priority queue in ready queue.
    // return: The pointer to the priority queue.
    ReadyQueue* getPriorityQueue(const int &priority){
                    return ready_queue_.at(priority);
    }
};

#endif
