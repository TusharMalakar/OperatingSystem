Author: Tushar Malakar
Project Distributed By: Professor Pavel Shostak
Project Purpose: Simulation of some functionalities of an Operating System

This project is a simulation of certain aspect of an operating system such as CPU scheduler, memory management, 
and I/O devices schedulers and queues.

This project is built with c++ 11 and you could built the executable by typing 
"make all" on the terminal where the the file is located. After successful compile, it will generate "simulator.o" file.

Type "./simulator" to run the simulation.

Also you could remove all the object file and the executable 
with the command "make clean".

Some invalid input will cause the program to display an error message and the program will exit, for example if you give 
a negative ram size the program won't appreciate that and the program will exit. The queues are ordered from top to bottom 
meaning that when the info in the queue are being printed out, the highest process will be closest to using the cpu.
 
For the memory table, the highest process being printed out meaning that it has been on the table the longest without 
being used. When a a entry must be deleted from the table, that highest one will be deleted.


At the start, the program asks the user two questions:

	1. How much RAM memory is there on the simulated computer? Your program receives the number in bytes (no kilobytes or words). I can enter any number up to 4000000000 (4 billions). Make sure you use datatypes that will not be overflown by such value.
	2. How many hard disks does the simulated computer have? The enumeration of the hard disks starts with 0. I am assuming between 1 to 10.

 
After these questions are answered, the simulation begins. The program constantly listens for the user inputs. 
The program will NOT ask for a confirmation to enter another input. The user inputs signal some system events. 
Your program simulates the corresponding system behavior.

The possible inputs are:
A: ‘A’ input means that a new process has been created.  //A <priority Number> <mem. size>
    When a new process arrives, your program should create its PCB and allocate memory for it’s first page (page #0).
    Also, when a new process is created your program should send it to the top level of the ready-queue or allow it to 
    use the CPU right away. When choosing a PID for the new process start from 1 and go up. Do NOT reuse PIDs of the 
    terminated processes.

Q: The currently running process has spent a time quantum using the CPU. If the same process continues to use the CPU and 
   one more Q command arrives, it means that the process has spent one more time quantum.

t: The process that is currently using the CPU terminates. It leaves the system immediately. Make sure you release the 
   memory used by this process. 

d number file_name: The process that currently uses the CPU requests the hard disk #number. It wants to read or write file 
                    file_name.

D number: The hard disk #number has finished the work for one process.

m address: The process that is currently using the CPU requests a memory operation for the logical address.

S r:  Shows what process is currently using the CPU and what processes are waiting in the ready-queue. 

S i:  Shows what processes are currently using the hard disks and what processes are waiting to use them.
      For each busy hard disk show the process that uses it and show its I/O-queue. Make sure to display the filenames 
      (from the d command) for each process. The enumeration of hard disks starts from 0.

S m:  Shows the state of memory. For each used frame display the process number that occupies it and the page 
      number stored in it. The enumeration of pages and frames starts from 0.