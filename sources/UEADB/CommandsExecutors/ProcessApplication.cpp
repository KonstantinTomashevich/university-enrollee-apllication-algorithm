#include "ProcessApplication.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Core/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <UEADB/Utils/ReferenceCountedList.hpp>
#include <iostream>

namespace UEADB
{
namespace ProcessApplication
{
unsigned ProcessApplication (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 0)
    {
        std::cout << "    Expected no argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::SharedPointer <UEAA::ReferenceCounted> universityPointer =
            sharedContext [SharedContextGlobalKeys::UNIVERSITY];

    if (universityPointer.GetTrackingObject () == nullptr)
    {
        std::cout << "    Can not find university in shared context!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }

    UEAA::University *university = static_cast <UEAA::University *> (universityPointer.GetTrackingObject ());
    std::list <UEAA::Enrollee *> excessEnrolleesList = university->ProcessEnrolleesApplication ();

    ReferenceCountedList <UEAA::SharedPointer <UEAA::Enrollee> > *excessEnrollees =
        new ReferenceCountedList <UEAA::SharedPointer <UEAA::Enrollee> > ();

    for (auto iterator = excessEnrolleesList.begin (); iterator != excessEnrolleesList.end (); iterator++)
    {
        excessEnrollees->data_.push_back (UEAA::SharedPointer <UEAA::Enrollee> (*iterator));
    }

    UEAA::SharedPointer <UEAA::ReferenceCounted> excessEnrolleesPointer (excessEnrollees);
    sharedContext.emplace (SharedContextGlobalKeys::EXCESS_ENROLLEES, excessEnrolleesPointer);
    return 0;
}

void PrintHelp ()
{
    std::cout << "ProcessApplication brief:" << std::endl <<
        "    Runs ProcessEnrolleesApplication of currently load university" << std::endl <<
        "    and stores excess enrollees to shared context." << std::endl;
}
}
}
