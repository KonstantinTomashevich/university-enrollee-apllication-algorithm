#pragma once
#include <map>
#include <string>
#include <list>

#include <UEAA/Utils/XMLSerializable.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/Enrollee/Enrollee.hpp>

namespace UEAA
{
class University : public XMLSerializable
{
public:
    University ();
    virtual ~University ();

    bool AddFaculty (Faculty *faculty);
    Faculty *GetFaculty (unsigned id) const;
    unsigned GetFacultiesCount () const;
    Faculty *GetFacultyByIndex (unsigned index) const;

    bool RemoveFaculty (unsigned id);
    void RemoveAllFaculties ();

    bool AddEnrollee (Enrollee *enrollee);
    Enrollee *GetEnrollee (unsigned id) const;
    Enrollee *GetEnrollee (const std::string &passportSeries, const std::string &passportNumber) const;

    unsigned GetEnrolleesCount () const;
    Enrollee *GetEnrolleeByIndex (unsigned index) const;

    bool RemoveEnrollee (unsigned id);
    void RemoveAllEnrollees ();

    void ClearEnroleesApplicationInfo ();
    /// Returns array of excess enrolees.
    std::list <Enrollee *> ProcessEnrolleesApplication ();

    virtual void SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const;
    virtual void LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable);

    bool operator == (const University &rhs) const;
    bool operator != (const University &rhs) const;

private:
    void ProcessEnroleesChoices (std::list <Enrollee *> &processing, std::list <Enrollee *> &excess) const;
    void AddFacultiesExcessToProcessingList (std::list <Enrollee *> &processing) const;

    std::map <unsigned, SharedPointer <Faculty> > faculties_;
    std::map <unsigned, SharedPointer <Enrollee> > enrollees_;
};
}
