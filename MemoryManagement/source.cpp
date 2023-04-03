#include <header.hpp>
#include <fstream>
#include <sstream>

FileParser::FileParser(std::string filename) { setFilename(filename); }

void FileParser::PrintLines() const
{
    std::for_each(lines.begin(), lines.end(), [](std::string item)
                  { std::cout << item << '\n'; });
}

void FileParser::setFilename(std::string filename) { this->filename = filename; }

std::string FileParser::getFilename() const { return filename; }

std::vector<std::string> &FileParser::getLines() { return lines; }

void FileParser::ParseLines()
{
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open())
    {
        std::cout << "[ERROR]: The file cannot be opened!\n";
        exit(EXIT_FAILURE);
    }

    std::string temp;
    while (std::getline(file, temp))
    {
        if (temp[0] != '\0')
            lines.push_back(temp);
    }
    lines.push_back("EOF");
}

ParseProcesses::ParseProcesses(std::vector<std::string> &lines) { this->lines = lines; }

void ParseProcesses::ParseOneProcess(std::queue<std::string> queue)
{
    std::vector<std::string> split_pr = SplitLine(queue.front());
    queue.pop();
    std::string pr_name = split_pr[1];

    Process *tmp = nullptr;

    for (Process *item : processes)
    {
        if (item->getPname() == pr_name)
        {
            tmp = item;
            break;
        }
    } 

    std::stringstream ss;
    int number = 0;

    if (tmp)
    {
        while (!queue.empty())
        {
            split_pr = SplitLine(queue.front());
            ss << split_pr[2];
            ss >> number;
            tmp->AddVariable(split_pr[1], number);
            ss.clear();
            queue.pop();
            variables.push_back({pr_name, split_pr[1], number});
        }

        return;
    }

    Process *pr = new Process(split_pr[1]);

    while (!queue.empty())
    {
        split_pr = SplitLine(queue.front());
        ss << split_pr[2];
        ss >> number;
        pr->AddVariable(split_pr[1], number);
        ss.clear();
        queue.pop();
        variables.push_back({pr_name, split_pr[1], number});
    }
    processes.push_back(pr);
}

void ParseProcesses::ParseAllProcess()
{
    std::vector<std::string> splitted;
    std::queue<std::string> temp;
    std::string first;

    // vector is used as queue.
    int index = 0;
    while (!lines.empty())
    {
        first = lines.front();
        lines.erase(lines.begin());
        splitted = SplitLine(first);
        if ((splitted[0] == "Process" || splitted[0] == "process" || splitted[0] == "EOF") && index > 0)
        {
            ParseOneProcess(temp); // create one process
            temp = std::queue<std::string>(); // free temp
            temp.push(first);
            index++;
            continue;
        }
        temp.push(first);
        index++;
    }
}

std::vector<Variable> ParseProcesses::getVariables() const { return variables; }

Process::Process(std::string pname) { this->pname = pname; }

bool Process::operator==(const Process &process) { return this->pname == process.pname; }

std::string Process::getPname() const { return pname; }

std::vector<std::pair<std::string, int>> Process::getVarval() const { return varval; }

int Process::getLastindex() const { return pagetable.size(); }

void Process::AddVariable(std::string variable, int value) { varval.push_back({variable, value}); }

void Process::AddPagetable(std::pair<int, int> addr) { pagetable.push_back(addr); }

void Process::AddVarAddress(std::pair<std::string, int> varaddr) { varaddress.push_back(varaddr); }

void Process::PrintPageTable() const
{
    std::cout << "Page Table of " << pname << '\n';
    for (std::pair<int, int> item : pagetable)
        std::cout << "\t\tPage " << item.first << ':' << item.second << '\n';
    std::cout << '\n';
}

void Process::PrintVarAddress() const
{
    std::for_each(varaddress.begin(), varaddress.end(), [](std::pair<std::string, int> item)
                  { std::cout << item.first << ' ' << item.second << '\n'; });
}

void Process::AssignVariables()
{
    int allocated = 0;
    int sizeofprocess = pagetable.size() * FRAME_SIZE;
    for (std::pair<std::string, int> &item : varval)
    {
        AddVarAddress({item.first, allocated});
        allocated += item.second;
    }
}

std::pair<int, int> Process::GetLogicalAddress(std::string varname)
{
    std::vector<std::pair<std::string, int>>::iterator it =
        std::find_if(varaddress.begin(), varaddress.end(), [&varname](std::pair<std::string, int> item)
                     { return item.first == varname; });
    int address = it->second;
    int pageaddress = address / FRAME_SIZE;
    int offset = address % FRAME_SIZE;
    std::pair<int, int> info = std::make_pair(pageaddress, offset);
    return info;
}

std::pair<int, int> Process::GetPhysicalAddress(std::string varname)
{
    std::pair<int, int> logicalAddress = GetLogicalAddress(varname);
    std::vector<std::pair<int, int>>::iterator it =
        std::find_if(pagetable.begin(), pagetable.end(), [&logicalAddress](std::pair<int, int> item)
                     { return logicalAddress.first == item.first; });

    int frame = it->second / FRAME_SIZE;
    std::pair<int, int> physicalAddress = std::make_pair(frame, logicalAddress.second);
    return physicalAddress;
}

std::vector<std::string> SplitLine(std::string line)
{
    std::vector<std::string> splitted;
    std::string word;
    std::stringstream stream(line);
    while (stream >> word)
        splitted.push_back(word);
    return splitted;
}
