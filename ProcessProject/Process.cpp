#include <Process.hpp>
vector<Process> processList;

Process::Process(int processID)
{
    this->processID = processID;
    this->state = READY;
    this->pc = 0;
    this->acc = 0.0;
    this->temp = 0.0;
    this->ir = "";
}

int Process::getProcessID() const
{
    return processID;
}

void Process::fetchInstructions(string filename)
{
    ifstream file(filename);

    if (!file.is_open())
        throw "File does not exist ! \n";
    string line = "";
    while (getline(file, line))
        instructionsQueue.push(line);
    file.close();
}

void Process::runProcess()
{
    if (state != BLOCKED)
    {
        ir = instructionsQueue.front();
        instructionsQueue.pop();
        state = RUNNING;

        string temp = ir;
        string *tokens = tokenizer(temp);
        int num = stoi(tokens[1]);
        if (tokens[0] == "load")
        {
            this->temp = num;
            acc = num;
        }
        else if (tokens[0] == "add")
        {
            this->temp = num;
            acc += num;
        }
        else if (tokens[0] == "sub")
        {
            this->temp = num;
            acc -= num;
        }
        else if (tokens[0] == "mul")
        {
            this->temp = num;
            acc *= num;
        }
        state = READY;
        pc++;
    }
}

void Process::blockProcess()
{
    state = BLOCKED;
}

void Process::unblockProcess()
{
    state = READY;
}

void Process::printContext() const
{
    cout << "Process ID : pr_" << this->processID << endl;
    cout << "Instruction Register : " << ir << endl
         << endl;
    cout << "Accumulator : " << acc << "\t\t"
         << "Temp : " << temp << endl;
    cout << "Program couter : " << pc << "\t\t"
         << "State : ";
    if (state == READY)
        cout << "ready" << endl;
    else
        cout << "blocked" << endl;
}

void create_process(int id, string filename)
{
    // cout << "create_process pr_" << id << ' ' << filename << endl;
    Process process(id);
    process.fetchInstructions(filename);
    processList.push_back(process);
}

void run_process(int id)
{
    // cout << "run_process pr_" << id << endl;
    Process *pr = searchProcess(id);
    if (pr == nullptr)
    {
        cout << "process doesn't exist" << endl;
        return;
    }
    pr->runProcess();
}

void block_process(int id)
{
    // cout << "block_process pr_" << id << endl;
    Process *pr = searchProcess(id);
    if (pr == nullptr)
    {
        cout << "process doesn't exist" << endl;
        return;
    }
    pr->blockProcess();
}

void unblock_process(int id)
{
    // cout << "unblock_process pr_" << id << endl;
    Process *pr = searchProcess(id);
    if (pr == nullptr)
    {
        cout << "process doesn't exist" << endl;
        return;
    }
    pr->unblockProcess();
}

void show_context(int id)
{
    // cout << "show_context pr_" << id << endl;
    Process *pr = searchProcess(id);
    if (pr == nullptr)
    {
        cout << "process doesn't exist" << endl;
        return;
    }
    pr->printContext();
}

void kill_process(int id)
{
    // cout << "kill_process pr_" << id << endl;
    int index = -1;
    for (int i = 0; i < processList.size(); i++)
    {
        if (processList[i].getProcessID() == id)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        cout << "process doesn't exist" << endl;
        return;
    }
    processList.erase(processList.begin() + index);
}

string *tokenizer(string &str)
{
    string delimiter = " ";
    string *array = new string[3];
    array[0] = "";
    array[1] = "";
    array[2] = "";

    size_t pos = 0;
    int i = 0;

    while ((pos = str.find(delimiter)) != string::npos)
    {
        array[i] = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        i++;
    }

    array[i] = str;

    return array;
}

Process *searchProcess(int id)
{
    for (int i = 0; i < processList.size(); i++)
    {
        if (processList[i].getProcessID() == id)
            return &processList[i];
    }
    return nullptr;
}
