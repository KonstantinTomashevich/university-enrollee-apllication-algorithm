#include <UEAA/BuildConfiguration.hpp>
#include "University.hpp"
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>
#include <UEAA/Utils/CStringToHash.hpp>

namespace UEAA
{
University::University () :
    XMLSerializable (),
    faculties_ (),
    enrollees_ ()
{

}

University::~University ()
{

}

bool University::AddFaculty (Faculty *faculty)
{
    if (!faculty)
    {
        return false;
    }

    if (faculties_.find (faculty->GetId ()) == faculties_.end ())
    {
        faculties_.emplace (faculty->GetId (), faculty);
        return true;
    }
    else
    {
        return false;
    }
}

Faculty *University::GetFaculty (unsigned id) const
{
    try
    {
        return faculties_.at (id);
    }
    catch (std::out_of_range &exception)
    {
        return nullptr;
    }
}

unsigned University::GetFacultiesCount () const
{
    return faculties_.size ();
}

Faculty *University::GetFacultyByIndex (unsigned index) const
{
    if (index < faculties_.size ())
    {
        auto iterator = faculties_.cbegin ();
        for (; index > 0; iterator++, index--) {}
        return iterator->second.GetTrackingObject ();
    }
    else
    {
        return nullptr;
    }
}

bool University::RemoveFaculty (unsigned id)
{
    auto iterator = faculties_.find (id);
    if (iterator == faculties_.end ())
    {
        return false;
    }

    faculties_.erase (iterator);
    return true;
}

void University::RemoveAllFaculties ()
{
    faculties_.clear ();
}

bool University::AddEnrollee (Enrollee *enrollee)
{
    if (!enrollee)
    {
        return false;
    }

    if (enrollees_.find (enrollee->GetId ()) == enrollees_.end ())
    {
        enrollees_.emplace (enrollee->GetId (), enrollee);
        return true;
    }
    else
    {
        return false;
    }
}

Enrollee *University::GetEnrollee (unsigned id) const
{
    try
    {
        return enrollees_.at (id);
    }
    catch (std::out_of_range &exception)
    {
        return nullptr;
    }
}

Enrollee *University::GetEnrollee (const std::string &passportId) const
{
    return GetEnrollee (CStringToHash (passportId.c_str ()));
}

unsigned University::GetEnrolleesCount () const
{
    return enrollees_.size ();
}

Enrollee *University::GetEnrolleeByIndex (unsigned index) const
{
    if (index < enrollees_.size ())
    {
        auto iterator = enrollees_.cbegin ();
        for (; index > 0; iterator++, index--) {}
        return iterator->second.GetTrackingObject ();
    }
    else
    {
        return nullptr;
    }
}

bool University::RemoveEnrollee (unsigned id)
{
    auto iterator = enrollees_.find (id);
    if (iterator == enrollees_.end ())
    {
        return false;
    }

    enrollees_.erase (iterator);
    return true;
}

void University::RemoveAllEnrollees ()
{
    enrollees_.clear ();
}

void University::ClearEnroleesApplicationInfo ()
{
    for (auto iterator = faculties_.begin (); iterator != faculties_.end (); iterator++)
    {
        iterator->second->ClearAllSpecialtiesEnrollees ();
    }

    for (auto iterator = enrollees_.begin (); iterator != enrollees_.end (); iterator++)
    {
        Enrollee *enrollee = iterator->second;
        enrollee->SetLastUpdateResult (nullptr);
        enrollee->RefreshChoiceIndex ();
    }
}

std::list <Enrollee *> University::ProcessEnrolleesApplication ()
{
    ClearEnroleesApplicationInfo ();
    std::list <Enrollee *> processing;
    std::list <Enrollee *> excessEnrolees;

    for (auto iterator = enrollees_.begin (); iterator != enrollees_.end (); iterator++)
    {
        processing.emplace_back (iterator->second);
    }

    while (!processing.empty ())
    {
        ProcessEnrolleesChoices (processing, excessEnrolees);
        AddFacultiesExcessToProcessingList (processing);
    }
    return excessEnrolees;
}

void University::SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetName ("university");
    tinyxml2::XMLElement *enrolleesElement = document.NewElement ("enrollees");
    output->InsertEndChild (enrolleesElement);

    for (auto iterator = enrollees_.cbegin (); iterator != enrollees_.cend (); iterator++)
    {
        Enrollee *enrollee = iterator->second;
        tinyxml2::XMLElement *enrolleeElement = document.NewElement ("enrollee");

        enrolleesElement->InsertEndChild (enrolleeElement);
        enrollee->SaveToXML (document, enrolleeElement, deHashTable);
    }

    tinyxml2::XMLElement *facultiesElement = document.NewElement ("faculties");
    output->InsertEndChild (facultiesElement);

    for (auto iterator = faculties_.cbegin (); iterator != faculties_.cend (); iterator++)
    {
        Faculty *faculty = iterator->second;
        tinyxml2::XMLElement *facultyElement = document.NewElement ("faculty");

        facultiesElement->InsertEndChild (facultyElement);
        faculty->SaveToXML (document, facultyElement, deHashTable);
    }
}

void University::LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable)
{
    RemoveAllEnrollees ();
    RemoveAllFaculties ();
    
    tinyxml2::XMLElement *enrolleesElement = input->FirstChildElement ("enrollees");
    if (enrolleesElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = enrolleesElement->FirstChildElement ("enrollee");
             element != nullptr; element = element->NextSiblingElement ("enrollee"))
        {
            std::string passport = element->Attribute ("passport");
            SharedPointer <Enrollee> enrollee (new Enrollee (CStringToHash (passport.c_str (), deHashTable)));
            enrollee->LoadFromXML (element, deHashTable);
            AddEnrollee (enrollee);
        }
    }

    tinyxml2::XMLElement *facultiesElement = input->FirstChildElement ("faculties");
    if (facultiesElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = facultiesElement->FirstChildElement ("faculty");
             element != nullptr; element = element->NextSiblingElement ("faculty"))
        {
            SharedPointer <Faculty> faculty (new Faculty (this, CStringToHash (element->Attribute ("name"), deHashTable)));
            faculty->LoadFromXML (element, deHashTable);
            AddFaculty (faculty);
        }
    }
}

bool University::operator == (const University &rhs) const
{
    if (enrollees_.size () == rhs.enrollees_.size ())
    {
        auto firstIterator = enrollees_.cbegin ();
        auto secondIterator = rhs.enrollees_.cbegin ();

        for (; firstIterator != enrollees_.cend () && secondIterator != rhs.enrollees_.cend ();
               firstIterator++, secondIterator++)
        {
            if (*firstIterator->second.GetTrackingObject () != *secondIterator->second.GetTrackingObject ())
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    if (faculties_.size () == rhs.faculties_.size ())
    {
        auto firstIterator = faculties_.cbegin ();
        auto secondIterator = rhs.faculties_.cbegin ();

        for (; firstIterator != faculties_.cend () && secondIterator != rhs.faculties_.cend ();
               firstIterator++, secondIterator++)
        {
            if (*firstIterator->second.GetTrackingObject () != *secondIterator->second.GetTrackingObject ())
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool University::operator != (const University &rhs) const
{
    return !(rhs == *this);
}

void University::ProcessEnrolleesChoices (std::list <Enrollee *> &processing, std::list <Enrollee *> &excess) const
{
    while (!processing.empty ())
    {
        Enrollee *enrollee = *processing.begin ();
        processing.pop_front ();
        bool isAdded = false;
        
        while (enrollee->HasMoreChoices () && !isAdded)
        {
            EnrolleeChoice *choice = enrollee->GetCurrentChoice ();
            Faculty *faculty = GetFaculty (choice->GetFaculty ());
            if (faculty)
            {
                Specialty *specialty = faculty->GetSpecialty (choice->GetSpecialty ());
                if (specialty)
                {
                    specialty->AddEnrollee (enrollee);
                    enrollee->SetLastUpdateResult (choice);
                    isAdded = true;
                }
                else
                {
                    enrollee->IncreaseChoiceIndex ();
                }
            }
            else
            {
                enrollee->IncreaseChoiceIndex ();
            }
        }

        if (!enrollee->HasMoreChoices ())
        {
            excess.emplace_back (enrollee);
            enrollee->SetLastUpdateResult (nullptr);
        }
    }
}

void University::AddFacultiesExcessToProcessingList (std::list <Enrollee *> &processing) const
{
    for (auto iterator = faculties_.begin (); iterator != faculties_.end (); iterator++)
    {
        Faculty *faculty = iterator->second;
        std::list <Enrollee *> excessInFaculty = faculty->GetExcessEnrollees ();

        for (auto excessInFacultyIterator = excessInFaculty.begin ();
             excessInFacultyIterator != excessInFaculty.end (); excessInFacultyIterator++)
        {
            Enrollee *enrollee = *excessInFacultyIterator;
            enrollee->IncreaseChoiceIndex ();
            processing.emplace_back (enrollee);
        }
    }
}
}
