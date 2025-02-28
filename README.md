# CS 3502 Project 1: Multi-Threaded Progamming and IPC

This repository includes the source code for Project 1: Multi-Theaded Programming and IPC for CS 3502: Operating Systems. It includes two main sub-projects:

* Project A: Multi-Threading Implementation - This project includes 4 phases that demonstrates the basic thread operations, resource protection, deadlock creation, and deadlock resolution.

* Project B: Inter-Process Communication (IPC) – This is a separate project that demonstrates how processes communicate through pipes, specifically the use of pipes to enable data transfer between parent and child processes.

# Building & Running the Programs

Operating System: Ubuntu Linux (version 24.10).
Language: C++
Compiler and Code Editor: g++ as the compiler, Visual Studio code as the code editor.
Dependencies/Packages: POSIX libraries, etc.

* Install g++ Compiler and VS Code from Terminal *

* g++ : sudo apt update && sudo apt install g++
* VS Code : sudo snap install --classic code

* Build & Compiling Instructions *

* Clone the repository:
* git clone https://github.com/annikandersson/CS_3502_Project1
* cd CS_3502_Project1
* Compile Project A & B:
* Open terminal ->
* g++ -std=c++11 projectA.cpp -o projectA
* g++ -std=c++11 projectB.cpp -o projectB
* Run programs:
* In terminal ->
* ./projectA
* ./projectB
