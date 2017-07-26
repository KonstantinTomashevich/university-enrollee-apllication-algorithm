#pragma once
#include <map>
#include <string>

#include <UEAA/Utils/ReferenceCounted.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/Enrollee/Enrollee.hpp>

namespace UEAA
{
class University : public ReferenceCounted
{
public:
    explicit University (const std::string &name);
    virtual ~University ();

    bool AddFaculty (Faculty *faculty);
    Faculty *GetFaculty (unsigned id) const;
    bool RemoveFaculty (unsigned id);
    void RemoveAllFaculties ();

    bool AddEnrollee (Enrollee *enrollee);
    Enrollee *GetEnrollee (unsigned id) const;
    bool RemoveEnrollee (unsigned id);
    void RemoveAllEnrollees ();

    void ClearEnrooleesApplicationInfo ();
    /// Returns array of unapplicateed enrolees.
    std::vector <Enrollee *> ProcessEnrolleesApplication ();

private:
    void ProcessEnroleesChoices (std::vector <Enrollee *> &processing, std::vector <Enrollee *> &excess) const;
    void ReaddExcessToProcessingList (std::vector <Enrollee *> &processing) const;

    std::string name_;
    std::map <unsigned, SharedPointer <Faculty> > faculties_;
    std::map <unsigned, SharedPointer <Enrollee> > enrollees_;
};
}
