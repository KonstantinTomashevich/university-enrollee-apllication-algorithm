#include "University.hpp"

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

void University::ClearEnrooleesApplicationInfo ()
{
    for (auto iterator = faculties_.begin (); iterator != faculties_.end (); iterator++)
    {
        iterator->second->ClearAllSpecialtiesEnrollees ();
    }

    for (auto iterator = enrollees_.begin (); iterator != enrollees_.end (); iterator++)
    {
        Enrollee *enrollee = iterator->second;
        enrollee->SetLastUpdateResult (EMPTY_ENROLLEE_CHOISE);
        enrollee->RefreshChoiseIndex ();
    }
}

std::vector <Enrollee *> University::ProcessEnrolleesApplication ()
{
    ClearEnrooleesApplicationInfo ();
    std::vector <Enrollee *> processing;
    std::vector <Enrollee *> excessEnrolees;

    for (auto iterator = enrollees_.begin (); iterator != enrollees_.end (); iterator++)
    {
        processing.push_back (iterator->second);
    }

    while (!processing.empty ())
    {
        ProcessEnroleesChoices (processing, excessEnrolees);
        processing.clear ();
        ReaddExcessToProcessingList (processing);
    }
    return excessEnrolees;
}

void University::ProcessEnroleesChoices (std::vector <Enrollee *> &processing, std::vector <Enrollee *> &excess) const
{
    for (auto iterator = processing.begin (); iterator != processing.end (); iterator++)
    {
        Enrollee *enrollee = *iterator;
        bool isAdded = false;
        while (enrollee->HasMoreChoises () && !isAdded)
        {
            EnrolleeChoise choise = enrollee->GetCurrentChoise ();
            Faculty *faculty = GetFaculty (choise.faculty_);
            if (faculty)
            {
                Specialty *specialty = faculty->GetSpecialty (choise.specialty_);
                if (specialty)
                {
                    specialty->AddEnrollee (enrollee);
                    enrollee->SetLastUpdateResult (choise);
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

        if (!enrollee->HasMoreChoises ())
        {
            excess.push_back (enrollee);
            enrollee->SetLastUpdateResult (EMPTY_ENROLLEE_CHOISE);
        }
    }
}

void University::ReaddExcessToProcessingList (std::vector <Enrollee *> &processing) const
{
    for (auto iterator = faculties_.begin (); iterator != faculties_.end (); iterator++)
    {
        Faculty *faculty = iterator->second;
        std::vector <Enrollee *> excessInFaculty = faculty->GetExcessEnrollees ();
        for (auto iterator = excessInFaculty.begin (); iterator != excessInFaculty.end (); iterator++)
        {
            Enrollee *enrollee = *iterator;
            enrollee->IncreaseChoiceIndex ();
            processing.push_back (enrollee);
        }
    }
}
}
