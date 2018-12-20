/*
  Title          : simulator.cpp
  Author         : Tushar Malakar
  Created on     : 20th November, 2018
  Description    : interface for the "PCB" class
  Purpose        : Class project
*/

#include <iostream>
#include <sstream>

#include "Memory.h"
using namespace std;

class Console{
public:
    // Default constructor
    Console() = default;
    // Default destructor
    ~Console() = default;

    // Runs the entire program; Processes user command requests.
    void run(){
    setupSystem();
    string command;
    int pid_tracker = 1;
    cout << "Starting simulation... For a full list of commands, type \"commands\".\n";
    cout << "Enter 'Q' or 'q' to quit.\n\n";


    do {
        cout << ">> ";
        getline(cin, command);
        if(command[0] == 'A'){
            my_system.newProcess(command, pid_tracker);
        } else if(command [0] == 'd') {
            my_system.requestIO(command);
        } else if (command[0] == 'D'){
            my_system.interruptDisk(command);
        } else if (command[0] == 'S'){
            my_system.snapshot(command);
        } else if (command[0] == 't'){
            my_system.terminate();
        } else if (command[0] == 'q' || command[0] == 'Q'){
            cout << "Thank you for using Priority Scheduler!\n";
            exit(1);
        } else if (command == "commands"){
            cout << commands();
        } else {
            cout << "Invalid command.\n";
        }
    } while(true);
}

private:
    // Instance of the system.
    System my_system;

    // Sets up the system information.
    // Gets user inputs for memory size, and number of disks.
    void setupSystem(){
    unsigned int num_disks, mem_size;
    string check;
    stringstream ss;

    cout << "    ============================================\n";
    cout << "   |       Welcome to Priority Scheduling       |\n";
    cout << "    ============================================\n\n";
    cout << "How much memory (in bytes) are in the system?\n>> ";
    getline(cin, check);
    //error handling
    while(!checkValidInput(check)){
        cout << "Please enter a valid memory size.\n>> ";
        getline(cin, check);
    }
    ss << check;
    ss >> mem_size;
    ss.clear();

    cout << "How many hard disks are there? (1-10)\n>> ";
    getline(cin, check);
    //error handling
    while(!checkValidInput(check)){
        cout << "Please enter a valid disk number (1-10).\n>> ";
        getline(cin, check);
    }
    ss << check;
    ss >> num_disks;
    while(num_disks <= 0 || num_disks > 10){
        cout << "Please enter a valid disk number (1-10).\n>> ";
        cin >> num_disks;
    }
    ss.clear();


    my_system.setupMemory(mem_size);
    my_system.setDisk(num_disks);
    cout << endl;
}

    // Creates a new process in the system.
    // param command: User command.
    // param pid_tracker: The current available ID for the process.
    void newProcess(string &command, int &pid_tracker);

    // Checks to see if the user entered in a valid input.
    // return: True if valid. Otherwise false.
    bool checkValidInput(string &input){
    unsigned int value;
    stringstream ss;
    ss << input;
    ss >> value;
    if(value == 0){
        return false;
    }
    return true;
}

    // Compares a given process and the current process in CPU.
    // Moves the given process to the CPU if priority is higher.
    // param process: The process to compare to the CPU process.
    void checkCPU(PCB* process);

    // Terminates the current process in CPU and gets the next process in ready queue.
    // If ready queue is empty, CPU is set to null.
    void terminate();

    // User manual for the program.
    // return: The string of all the manual information.
    string commands(){
    stringstream ss;
    ss << "Command Information:\n"
        << "\tA <priority level> <memory size> : Creating a new process\n "
        << "\tt : Terminate current process in CPU\n"
        << "\td <disk number> : Process in CPU requests <disk number>\n"
        << "\tD <disk number> : Interrupt from <disk number>. Process finished task.\n"
        << "\tS <i, r, OR m> : Snapshot of System\n"
        << "\t\t S i : IO information\n"
        << "\t\t S r : Ready Queue information\n"
        << "\t\t S m : Memory information\n"
        << "\t'Q' or 'q' to exit program.\n";
        return ss.str();
 }
};




int main(int argc, const char * argv[]){
    if (argc != 1) {
        std::cout << "Usage: /." << argv[0] << std::endl;
        return 1;
    }
    Console OS;
    OS.run();

    return 0;
}







