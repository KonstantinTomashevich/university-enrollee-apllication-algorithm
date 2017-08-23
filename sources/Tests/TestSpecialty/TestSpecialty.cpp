#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/University/Specialty.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <iostream>
#include <cmath>
#include <time.h>

const unsigned MATH_EXAM = UEAA::CStringToHash ("Math");
const unsigned PHYSICS_EXAM = UEAA::CStringToHash ("Physics");
const unsigned LANGUAGE_EXAM = UEAA::CStringToHash ("Language");
const unsigned SPECIALTY_ID = 1;

const unsigned GENERATE_FOR_FREE_FORM = 15;
const unsigned GENERATE_FOR_PAID_FORM = 10;
const unsigned MAX_IN_FREE_FORM = 10;
const unsigned MAX_IN_PAID_FORM = 5;

void InitSpecialty (UEAA::Specialty *specialty);
UEAA::Enrollee *GenerateEnrollee (UEAA::StudyForm studyForm);
void PrintEnrollee (const UEAA::Enrollee *enrollee);
void PrintEnrolleesVector (const std::vector <UEAA::Enrollee *> &enrollees);

int main ()
{
    srand (time (0));
    std::vector <UEAA::SharedPointer <UEAA::Enrollee> > enrolleesBuffer;

    UEAA::SharedPointer <UEAA::Specialty> specialty (new UEAA::Specialty (SPECIALTY_ID));
    InitSpecialty (specialty);

    std::cout << "GENERATED ENROLLEES FOR FREE FORM:" << std::endl;
    for (unsigned index = 0; index < GENERATE_FOR_FREE_FORM; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee = GenerateEnrollee (UEAA::STUDY_FORM_FREE);
        PrintEnrollee (enrollee);
        enrolleesBuffer.push_back (enrollee);
        if (!specialty->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!";
            return 1;
        }
    }

    std::cout << "GENERATED ENROLLEES FOR PAID FORM:" << std::endl;
    for (unsigned index = 0; index < GENERATE_FOR_PAID_FORM; index++)
    {
        UEAA::SharedPointer <UEAA::Enrollee> enrollee = GenerateEnrollee (UEAA::STUDY_FORM_PAID);
        PrintEnrollee (enrollee);
        enrolleesBuffer.push_back (enrollee);
        if (!specialty->AddEnrollee (enrollee))
        {
            std::cout << "Can't add enrollee!";
            return 1;
        }
    }

    std::cout << std::endl;
    std::cout << "Choose free form: " << specialty->GetEnrolleesInFreeForm ().size () << std::endl;
    std::cout << "Choose paid form: " << specialty->GetEnrolleesInPaidForm ().size () << std::endl;

    if (specialty->GetEnrolleesInFreeForm ().size () != GENERATE_FOR_FREE_FORM)
    {
        std::cout << "Incorrect enrollees count in free form (before removing)!" << std::endl;
        return 1;
    }

    if (specialty->GetEnrolleesInPaidForm ().size () != GENERATE_FOR_PAID_FORM)
    {
        std::cout << "Incorrect enrollees count in paid form (before removing)!" << std::endl;
        return 1;
    }

    std::cout << "Removing excess enrollees..." << std::endl;
    std::vector <UEAA::Enrollee *> excessEnrollees = specialty->RemoveExcessEnrollees ();
    std::cout << "In free form: " << specialty->GetEnrolleesInFreeForm ().size () << std::endl;
    std::cout << "In paid form: " << specialty->GetEnrolleesInPaidForm ().size () << std::endl;

    if (specialty->GetEnrolleesInFreeForm ().size () != MAX_IN_FREE_FORM)
    {
        std::cout << "Incorrect enrollees count in free form (after removing)!" << std::endl;
        return 1;
    }

    if (specialty->GetEnrolleesInPaidForm ().size () != MAX_IN_PAID_FORM)
    {
        std::cout << "Incorrect enrollees count in paid form (after removing)!" << std::endl;
        return 1;
    }

    std::cout << "ENROLLEES IN FREE FORM:" << std::endl;
    PrintEnrolleesVector (specialty->GetEnrolleesInFreeForm ());

    std::cout << "ENROLLEES IN PAID FORM:" << std::endl;
    PrintEnrolleesVector (specialty->GetEnrolleesInPaidForm ());

    for (auto iterator = excessEnrollees.cbegin (); iterator != excessEnrollees.cend (); iterator++)
    {
        const UEAA::Enrollee *excessEnrollee = *iterator;
        const std::vector <UEAA::Enrollee *> anotherEnrollees =
                (excessEnrollee->GetCurrentChoice ().studyForm_ == UEAA::STUDY_FORM_FREE) ?
                    specialty->GetEnrolleesInFreeForm () : specialty->GetEnrolleesInPaidForm ();
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
    specialty->SetMaxEnrolleesInFreeForm (MAX_IN_FREE_FORM);
    specialty->SetMaxEnrolleesInPaidForm (MAX_IN_PAID_FORM);

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

UEAA::Enrollee *GenerateEnrollee (UEAA::StudyForm studyForm)
{
    UEAA::Enrollee *enrollee = new UEAA::Enrollee (UEAA::CalculateEnrolleeHash ("XX", "0000000"));
    enrollee->SetCertificateMark (MATH_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (PHYSICS_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (LANGUAGE_EXAM, 5 + rand () % 6);
    enrollee->CalculateCertificateMedianMark ();

    enrollee->SetExamResult (MATH_EXAM, 30 + rand () % 71);
    enrollee->SetExamResult (PHYSICS_EXAM, 30 + rand () % 71);
    enrollee->SetExamResult (LANGUAGE_EXAM, 30 + rand () % 71);

    enrollee->AddChoiceToBack (UEAA::EnrolleeChoice (0, SPECIALTY_ID, studyForm));
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

void PrintEnrolleesVector (const std::vector <UEAA::Enrollee *> &enrollees)
{
    for (auto iterator = enrollees.cbegin (); iterator != enrollees.cend (); iterator++)
    {
        PrintEnrollee (*iterator);
    }
}
