#pragma once
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <cstdlib>
#include <string>
#include <vector>

namespace UEADB
{
class Application : public UEAA::ReferenceCounted
{
public:
    Application ();
    virtual ~Application ();

    unsigned Execute (const wchar_t* arguments);
    unsigned Execute (int argumentsCount, char **arguments);

private:
    unsigned Execute (const std::vector <std::string> &arguments);
    std::vector <std::string> ParseArgumentsString (const wchar_t *arguments);
};
}
