#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
using namespace std;

enum State
{
    READY,
    RUNNING,
    BLOCKED
};

class Process
{
private:
    int processID;
    State state;
    int pc;
    float acc;
    float temp;
    string ir;
    queue<string> instructionsQueue;

public:
    Process(int id);
    ~Process() = default;
    int getProcessID() const;
    void fetchInstructions(string filename);
    void runProcess();
    void blockProcess();
    void unblockProcess();
    void printContext() const;
};

void create_process(int id, string filename);
void run_process(int id);
void block_process(int id);
void unblock_process(int id);
void show_context(int id);
void kill_process(int id);

string *tokenizer(string &str);
Process *searchProcess(int id);
extern vector<Process> processList;

#endif