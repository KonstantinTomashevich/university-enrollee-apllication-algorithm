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
    const std::string &GetName () const;

    bool AddFaculty (Faculty *faculty);
    Faculty *GetFaculty (unsigned id) const;
    bool RemoveFaculty (unsigned id);
    void RemoveAllFaculties ();

    bool AddEnrollee (Enrollee *enrollee);
    Enrollee *GetEnrollee (unsigned id) const;
    Enrollee *GetEnrollee (const std::string &passportSeries, const std::string &passportNumber) const;
    bool RemoveEnrollee (unsigned id);
    void RemoveAllEnrollees ();

    void ClearEnroleesApplicationInfo ();
    /// Returns array of excess enrolees.
    std::vector <Enrollee *> ProcessEnrolleesApplication ();

private:
    void ProcessEnroleesChoices (std::vector <Enrollee *> &processing, std::vector <Enrollee *> &excess) const;
    void AddExcessToProcessingList (std::vector <Enrollee *> &processing) const;

    std::string name_;
    std::map <unsigned, SharedPointer <Faculty> > faculties_;
    std::map <unsigned, SharedPointer <Enrollee> > enrollees_;
};
}
