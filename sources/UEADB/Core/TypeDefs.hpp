#pragma once
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <vector>
#include <map>
#include <string>

namespace UEADB
{
typedef std::pair <std::string, std::vector <std::string>> CommandInfo;
typedef std::vector <CommandInfo> CommandsList;
typedef std::map <unsigned, UEAA::SharedPointer <UEAA::ReferenceCounted> > SharedPointersMap;
typedef unsigned (*CommandExecutor) (const std::vector <std::string> &, const SharedPointersMap &);
}
