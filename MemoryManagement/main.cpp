#include <iostream>
#include <cmath>
#include <header.hpp>

static int current_frame = 0;
static int allocated = 0;
std::vector<Process *> processes;
static char memory[MEMORY_SIZE] = {0};

static void fill_pagetable(std::vector<Variable> &vars);
static Process *find_process_by_name(std::string pname);
inline void check_memorysize_constraint();
static void print_addresses(std::vector<std::string> &command);

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: OSProject.exe <filename>\n";
        exit(EXIT_FAILURE);
    }

    std::string filename(argv[1]);
    FileParser file(filename);
    file.ParseLines();
    
    // lines were got in lines vector!
    
    ParseProcesses parse(file.getLines());
    parse.ParseAllProcess();
    std::vector<Variable> temp = parse.getVariables();

    fill_pagetable(temp);

    for (Process *item : processes)
        item->AssignVariables();
    
    std::pair<int, int> address;
    Process *pr = nullptr;

    std::cout << "[LOG]: The processes were loaded\n";
    system("pause");

    std::string command = "";
    std::cout << "Command format: Request <variable name> <process name>\n";
    std::cout << "Enter the command: ";
    std::getline(std::cin, command);

    std::vector<std::string> split_command = SplitLine(command);

    while (split_command[0] != "exit") {
        print_addresses(split_command);
        std::cout << "Enter the command: ";
        std::getline(std::cin, command);
        split_command = SplitLine(command);
    }

    return 0;
}

void fill_pagetable(std::vector<Variable> &vars)
{
    std::deque<int> indices;
    for (int i = 1; i < vars.size(); i++)
        if (vars[i].pname != vars[i - 1].pname)
            indices.push_back(i);
    indices.push_back(vars.size());

    Process *process;
    int a = 0, b = 0, total = 0, pages_need = 0;
    while (!indices.empty())
    {
        b = indices.front();
        indices.pop_front();

        for (int i = a; i < b; i++)
            total += vars[i].size;
        pages_need = ceil((double)total / (double)PAGE_SIZE);
        process = find_process_by_name(vars[a].pname);
        for (int i = 0; i < pages_need; i++)
        {
            current_frame += i * PAGE_SIZE;
            process->AddPagetable({process->getLastindex(), current_frame});
        }
        current_frame += FRAME_SIZE;
        check_memorysize_constraint();
        // std::cout << vars[a].pname << ' ' << total << '\n';
        a = b;
        total = 0;
    }
}

Process *find_process_by_name(std::string pname)
{
    std::vector<Process *>::iterator result = std::find_if(processes.begin(), processes.end(), [pname](Process const *item)
                                                           { return item->getPname() == pname; });
    Process *res = *result;
    if (res->getPname() == pname)
        return res;
    return nullptr;
}

void check_memorysize_constraint()
{
    if (current_frame > 4000) {
        std::cout << "[ERROR]: You cannot use memory over 4000 bytes!\n";
        exit(EXIT_FAILURE);
    }
}

void print_addresses(std::vector<std::string> &command)
{
    if (command.size() < 3) {
        std::cout << "Command format is wrong\n";
        return;
    }

    Process *pr = find_process_by_name(command[2]);
    std::pair<int, int> address;
    
    address = pr->GetLogicalAddress(command[1]);
    std::cout << "Page " << address.first << " Offset " << address.second << '\n';
    address = pr->GetPhysicalAddress(command[1]);
    std::cout << "Frame " << address.first << " Offset " << address.second << '\n';
}
