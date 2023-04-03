#ifndef HEADER_HPP
#define HEADER_HPP
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <iostream>
#define MEMORY_SIZE 4000
#define FRAME_SIZE 400
#define PAGE_SIZE 400

std::vector<std::string> SplitLine(std::string line);

struct Variable
{
    std::string pname;
    std::string varname;
    int size;
};

class FileParser
{
private:
    std::string filename;
    std::vector<std::string> lines;

public:
    FileParser(std::string filename);
    ~FileParser() = default;
    void PrintLines() const;

    void setFilename(std::string filename);
    std::string getFilename() const;
    std::vector<std::string> &getLines();

    void ParseLines();
};

class Process
{
private:
    std::string pname;
    std::vector<std::pair<std::string, int>> varval;
    std::vector<std::pair<int, int>> pagetable;
    std::vector<std::pair<std::string, int>> varaddress;

public:
    Process(std::string pname);
    ~Process() = default;

    bool operator==(const Process &process);

    std::string getPname() const;
    std::vector<std::pair<std::string, int>> getVarval() const;
    int getLastindex() const; // return size of page table

    void AddVariable(std::string variable, int value);
    void AddPagetable(std::pair<int, int> addr); // map page index with frame number
    void AddVarAddress(std::pair<std::string, int> varaddr);
    void PrintPageTable() const;
    void PrintVarAddress() const;
    void AssignVariables(); // put variables in page table

    std::pair<int, int> GetLogicalAddress(std::string varname);
    std::pair<int, int> GetPhysicalAddress(std::string varname);
};

class ParseProcesses
{
private:
    std::vector<std::string> lines;
    std::vector<Variable> variables;

public:
    ParseProcesses(std::vector<std::string> &lines);
    ~ParseProcesses() = default;

    std::vector<Variable> getVariables() const;

    void ParseOneProcess(std::queue<std::string> queue);
    void ParseAllProcess();
};

extern std::vector<Process *> processes;

#endif