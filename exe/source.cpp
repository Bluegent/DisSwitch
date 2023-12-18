#include "WinapiHelper.hpp"
#include <iomanip>

const std::string cloneCommand("clone");
const std::string extendCommand("extend");
const std::string disconnectCommand("disc");
const std::string listCommand("list");
const char* tab = "    ";

void help()
{
    std::cout << "Commands:\n";
    std::cout << tab << std::setw(6) << cloneCommand << " - connects display 4 and clones display 2 onto it\n";
    std::cout << tab << std::setw(6) << extendCommand << " - connects display 4 in extended mode\n";
    std::cout << tab << std::setw(6) << disconnectCommand << " - disconnects display 4\n";
    std::cout << tab << std::setw(6) << listCommand << " - lists display info\n\n";
    std::cout << "All the above assume you have 4 displays connected because this program is for my super specific setup :)\n";
}

void error(const char* reason)
{
    std::cerr << "Oh no babby what IS you doign??? (" << reason << ")\n";
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        help();
        return 0;
    }

    if (argc != 2)
    {
        error("incorrect argument count");
        return -1;
    }
    WinapiHelper helper;

    if (!helper.isCorrectDisplayCount())
    {
        error("incorrect display count");
        return -1;
    }
    helper.list();
    std::string command(argv[1]);

    if (command == cloneCommand)
    {
        helper.clone();
    }
    else if (command == extendCommand)
    {
        helper.extend();
    }
    else if (command == disconnectCommand)
    {
        helper.disconnect();
    }
    else if (command == listCommand)
    {
        helper.list();
    }
    else
    {
        error("unknown command");
    }

    return 0;
}
