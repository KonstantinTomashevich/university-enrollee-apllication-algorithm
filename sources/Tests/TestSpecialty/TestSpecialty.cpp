#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/University/Specialty.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <iostream>
#include <cmath>
#include <ctime>

const unsigned MATH_EXAM = UEAA::CStringToHash ("Math");
const unsigned PHYSICS_EXAM = UEAA::CStringToHash ("Physics");
const unsigned LANGUAGE_EXAM = UEAA::CStringToHash ("Language");
const unsigned SPECIALTY_ID = 1;

const unsigned GENERATE_ENROLLEES = 15;
const unsigned MAX_ENROLLEES = 10;

void InitSpecialty (UEAA::Specialty *specialty);
UEAA::Enrollee *GenerateEnrollee ();
void PrintEnrollee (const UEAA::Enrollee *enrollee);
void PrintEnrolleesList (const std::list <UEAA::Enrollee *> &enrollees);

int main ()
{
    srand (time (0));
    std::vector <UEAA::SharedPointer <UEAA::Enrollee> > enrolleesBuffer;

    UEAA::SharedPointer <UEAA::Specialty> specialty (new UEAA::Specialty (0, SPECIALTY_ID));
    InitSpecialty (specialty);

    std::cout << "GENERATED ENROLLEES:" << std::endl;
    for (unsigned index = 0; index < GENERATE_ENROLLEES; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee = GenerateEnrollee ();
        PrintEnrollee (enrollee);
        enrolleesBuffer.push_back (enrollee);
        if (!specialty->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!";
            return 1;
        }
    }

    std::cout << std::endl;
    std::cout << "Choose: " << specialty->GetEnrollees ().size () << std::endl;
    if (specialty->GetEnrollees ().size () != GENERATE_ENROLLEES)
    {
        std::cout << "Incorrect enrollees count in free form (before removing)!" << std::endl;
        return 1;
    }

    std::cout << "Removing excess enrollees..." << std::endl;
    std::list <UEAA::Enrollee *> excessEnrollees = specialty->RemoveExcessEnrollees ();
    std::cout << "After removing: " << specialty->GetEnrollees ().size () << std::endl;

    if (specialty->GetEnrollees ().size () != MAX_ENROLLEES)
    {
        std::cout << "Incorrect enrollees count in free form (after removing)!" << std::endl;
        return 1;
    }

    std::cout << "ENROLLEES IN SPECIALTY:" << std::endl;
    PrintEnrolleesList (specialty->GetEnrollees ());

    for (auto iterator = excessEnrollees.cbegin (); iterator != excessEnrollees.cend (); iterator++)
    {
        const UEAA::Enrollee *excessEnrollee = *iterator;
        const std::list <UEAA::Enrollee *> anotherEnrollees = specialty->GetEnrollees ();

        for (auto anotherEnrolleesIterator = anotherEnrollees.cbegin ();
             anotherEnrolleesIterator != anotherEnrollees.cend (); anotherEnrolleesIterator++)
        {
            const UEAA::Enrollee *anotherEnrollee = *anotherEnrolleesIterator;
            if (UEAA::IsFirstEnrolleeBetter (specialty, excessEnrollee, anotherEnrollee))
            {
                std::cout << "Error! Better enrollee isn't applied!" << std::endl << "Better enrollee:" << std::endl;
                PrintEnrollee (excessEnrollee);
                std::cout << "Another enrollee:" << std::endl;
                PrintEnrollee (anotherEnrollee);
                return 1;
            }
        }
    }

    return 0;
}

void InitSpecialty (UEAA::Specialty *specialty)
{
    specialty->SetMaxEnrollees (MAX_ENROLLEES);
    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {MATH_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {PHYSICS_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (false, {LANGUAGE_EXAM}));
        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (MATH_EXAM);
        certPriority.push_back (PHYSICS_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
}

UEAA::Enrollee *GenerateEnrollee ()
{
    UEAA::Enrollee *enrollee = new UEAA::Enrollee (UEAA::CStringToHash ("XX0000000"));
    enrollee->SetCertificateMark (MATH_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (PHYSICS_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (LANGUAGE_EXAM, 5 + rand () % 6);
    enrollee->CalculateCertificateMedianMark ();

    enrollee->SetExamResult (MATH_EXAM, 30 + rand () % 71);
    enrollee->SetExamResult (PHYSICS_EXAM, 30 + rand () % 71);
    enrollee->SetExamResult (LANGUAGE_EXAM, 30 + rand () % 71);

    enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (0, SPECIALTY_ID));
    return enrollee;
}

void PrintEnrollee (const UEAA::Enrollee *enrollee)
{
    std::cout << "Enrollee:" << std::endl <<
                 "    Median:   " << enrollee->GetCertificateMedianMark () * 1 << std::endl <<
                 "    Math:     " << enrollee->GetExamResult (MATH_EXAM) * 1 << std::endl <<
                 "    Physics:  " << enrollee->GetExamResult (PHYSICS_EXAM) * 1 << std::endl <<
                 "    Language: " << enrollee->GetExamResult (LANGUAGE_EXAM) * 1 << std::endl <<
                 std::endl;
}

void PrintEnrolleesList (const std::list <UEAA::Enrollee *> &enrollees)
{
    for (auto iterator = enrollees.cbegin (); iterator != enrollees.cend (); iterator++)
    {
        PrintEnrollee (*iterator);
    }
}
