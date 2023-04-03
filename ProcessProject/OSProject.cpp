#include <Process.hpp>
#include <map>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        cout << "help: main.exe <filename>" << endl;
        return EXIT_FAILURE;
    }

    queue<string> fileLines;
    ifstream commands(argv[1]);

    if (!commands.is_open())
    {
        cout << "File does not exist ! \n";
        return EXIT_FAILURE;
    }

    // every thing goes well until here!

    map<string, void (*)(int)> signals;
    signals["run_process"] = &run_process;
    signals["block_process"] = &block_process;
    signals["unblock_process"] = &unblock_process;
    signals["show_context"] = &show_context;
    signals["kill_process"] = &kill_process;

    string line = "";
    while (getline(commands, line))
        fileLines.push(line);

    int loop = 0, number = 0;
    string *tokens = nullptr;
    while (!fileLines.empty())
    {
        cout << "loop :  " << loop << " -----------------------------------------------------------------" << endl;
        loop++;
        line = fileLines.front();
        fileLines.pop();

        tokens = tokenizer(line);

        number = stoi(tokens[1].substr(3));
        if (tokens[0] == "create_process")
        {
            string address = "tests\\" + tokens[2];
            create_process(number, address);
        }
        else
        {
            void (*func)(int) = signals[tokens[0]];
            func(number);
        }
    }

    return 0;
}

// Ali Nikaein & Mohammad Hadi Hoghoughi