#include "University.hpp"
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

namespace UEAA
{
University::University (const std::string &name) :
    ReferenceCounted (),
    name_ (name),
    faculties_ (),
    enrollees_ ()
{

}

University::~University ()
{

}

const std::string &University::GetName () const
{
    return name_;
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
        return 0;
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
        return 0;
    }
}

Enrollee *University::GetEnrollee (const std::string &passportSeries, const std::string &passportNumber) const
{
    return GetEnrollee (CalculateEnrolleeHash (passportSeries, passportNumber));
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
        enrollee->SetLastUpdateResult (EMPTY_ENROLLEE_CHOICE);
        enrollee->RefreshChoiceIndex ();
    }
}

std::vector <Enrollee *> University::ProcessEnrolleesApplication ()
{
    ClearEnroleesApplicationInfo ();
    std::vector <Enrollee *> processing;
    std::vector <Enrollee *> excessEnrolees;

    for (auto iterator = enrollees_.begin (); iterator != enrollees_.end (); iterator++)
    {
        processing.push_back (iterator->second.GetTrackingObject ());
    }

    while (!processing.empty ())
    {
        ProcessEnroleesChoices (processing, excessEnrolees);
        processing.clear ();
        AddExcessToProcessingList (processing);
    }
    return excessEnrolees;
}

void University::ProcessEnroleesChoices (std::vector <Enrollee *> &processing, std::vector <Enrollee *> &excess) const
{
    for (auto iterator = processing.begin (); iterator != processing.end (); iterator++)
    {
        Enrollee *enrollee = *iterator;
        bool isAdded = false;
        while (enrollee->HasMoreChoices () && !isAdded)
        {
            EnrolleeChoice choice = enrollee->GetCurrentChoice ();
            Faculty *faculty = GetFaculty (choice.faculty_);
            if (faculty)
            {
                Specialty *specialty = faculty->GetSpecialty (choice.specialty_);
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
            excess.push_back (enrollee);
            enrollee->SetLastUpdateResult (EMPTY_ENROLLEE_CHOICE);
        }
    }
}

void University::AddExcessToProcessingList (std::vector <Enrollee *> &processing) const
{
    for (auto iterator = faculties_.begin (); iterator != faculties_.end (); iterator++)
    {
        Faculty *faculty = iterator->second;
        std::vector <Enrollee *> excessInFaculty = faculty->GetExcessEnrollees ();
        for (auto excessInFacultyIterator = excessInFaculty.begin ();
             excessInFacultyIterator != excessInFaculty.end (); excessInFacultyIterator++)
        {
            Enrollee *enrollee = *excessInFacultyIterator;
            enrollee->IncreaseChoiceIndex ();
            processing.push_back (enrollee);
        }
    }
}
}
