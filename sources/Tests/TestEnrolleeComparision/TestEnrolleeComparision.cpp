#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/University/Specialty.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <iostream>
#include <cmath>

const unsigned MATH_EXAM = UEAA::CStringToHash ("Math");
const unsigned PHYSICS_EXAM = UEAA::CStringToHash ("Physics");
const unsigned BELARUSIAN_LANGUAGE_EXAM = UEAA::CStringToHash ("BelarusianLanguage");
const unsigned RUSSIAN_LANGUAGE_EXAM = UEAA::CStringToHash ("RussianLanguage");

void InitSpecialty (UEAA::Specialty *specialty);
int main ()
{
    UEAA::SharedPointer <UEAA::Specialty> specialty (new UEAA::Specialty (0, 1));
    InitSpecialty (specialty);

    UEAA::SharedPointer <UEAA::Enrollee> enrollee1 (new UEAA::Enrollee (UEAA::CalculateEnrolleeHash ("XX", "0000000")));
    UEAA::SharedPointer <UEAA::Enrollee> enrollee2 (new UEAA::Enrollee (UEAA::CalculateEnrolleeHash ("YY", "1111111")));

    enrollee1->SetCertificateMark (MATH_EXAM, 9);
    enrollee1->SetCertificateMark (PHYSICS_EXAM, 9);
    enrollee1->SetCertificateMark (RUSSIAN_LANGUAGE_EXAM, 8);
    enrollee1->SetCertificateMark (BELARUSIAN_LANGUAGE_EXAM, 8);
    enrollee1->CalculateCertificateMedianMark ();
    std::cout << "Enrollee 1 median mark: " << enrollee1->GetCertificateMedianMark () << std::endl;

    enrollee2->SetCertificateMark (MATH_EXAM, 10);
    enrollee2->SetCertificateMark (PHYSICS_EXAM, 8);
    enrollee2->SetCertificateMark (RUSSIAN_LANGUAGE_EXAM, 8);
    enrollee2->SetCertificateMark (BELARUSIAN_LANGUAGE_EXAM, 9);
    enrollee2->CalculateCertificateMedianMark ();
    std::cout << "Enrollee 2 median mark: " << enrollee2->GetCertificateMedianMark () << std::endl;

    enrollee1->SetExamResult (MATH_EXAM, 80);
    enrollee1->SetExamResult (PHYSICS_EXAM, 78);
    enrollee1->SetExamResult (RUSSIAN_LANGUAGE_EXAM, 70);

    enrollee2->SetExamResult (MATH_EXAM, 90);
    enrollee2->SetExamResult (PHYSICS_EXAM, 71);
    enrollee2->SetExamResult (RUSSIAN_LANGUAGE_EXAM, 65);
    enrollee2->SetExamResult (BELARUSIAN_LANGUAGE_EXAM, 75);

    std::cout << "Enrollee 1 total score: " << UEAA::CalculateEnrolleeScore (specialty, enrollee1) << std::endl;
    std::cout << "Enrollee 2 total score: " << UEAA::CalculateEnrolleeScore (specialty, enrollee2) << std::endl;

    bool comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee1, enrollee2);
    std::cout << "Is first enrollee better than second: " << comparisionResult << std::endl;
    if (comparisionResult)
    {
        std::cout << "Error in enrollees comparision №1!" << std::endl;
        return 1;
    }

    std::cout << "Resetting enrollee 2 results." << std::endl;
    enrollee2->SetExamResult (MATH_EXAM, 81);
    enrollee2->SetExamResult (BELARUSIAN_LANGUAGE_EXAM, 73);
    std::cout << "Enrollee 2 total score: " << UEAA::CalculateEnrolleeScore (specialty, enrollee2) << std::endl;

    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee2, enrollee1);
    std::cout << "Is second enrollee better that first: " << comparisionResult << std::endl;
    if (!comparisionResult)
    {
        std::cout << "Error in enrollees comparision №2!" << std::endl;
        return 1;
    }

    std::cout << "Resetting enrollee 2 results." << std::endl;
    enrollee2->SetExamResult (MATH_EXAM, 80);
    enrollee2->SetExamResult (PHYSICS_EXAM, 78);
    enrollee2->SetExamResult (BELARUSIAN_LANGUAGE_EXAM, 67);
    std::cout << "Enrollee 2 total score: " << UEAA::CalculateEnrolleeScore (specialty, enrollee2) << std::endl;

    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee1, enrollee2);
    std::cout << "Is first enrollee better than second: " << comparisionResult << std::endl;
    if (comparisionResult)
    {
        std::cout << "Error in enrollees comparision №3!" << std::endl;
        return 1;
    }

    std::cout << "Adding school gold medal to first enrollee." << std::endl;
    enrollee1->SetHasSchoolGoldMedal (true);
    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee2, enrollee1);
    std::cout << "Is second enrollee better than first: " << comparisionResult << std::endl;
    if (comparisionResult)
    {
        std::cout << "Error in enrollees comparision №4!" << std::endl;
        return 1;
    }

    enrollee1->SetHasSchoolGoldMedal (false);
    enrollee1->SetRODSubject (MATH_EXAM);
    enrollee1->SetRODType (UEAA::ROD_SECOND);

    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee1, enrollee2);
    std::cout << "Is first enrollee better than second: " << comparisionResult << std::endl;
    if (!comparisionResult)
    {
        std::cout << "Error in enrollees comparision №5!" << std::endl;
        return 1;
    }

    enrollee2->SetRODSubject (BELARUSIAN_LANGUAGE_EXAM);
    enrollee2->SetRODType (UEAA::ROD_FIRST);
    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee1, enrollee2);
    std::cout << "Is first enrollee better than second: " << comparisionResult << std::endl;
    if (!comparisionResult)
    {
        std::cout << "Error in enrollees comparision №6!" << std::endl;
        return 1;
    }

    enrollee2->SetRODSubject (MATH_EXAM);
    comparisionResult = UEAA::IsFirstEnrolleeBetter (specialty, enrollee1, enrollee2);
    std::cout << "Is first enrollee better than second: " << comparisionResult << std::endl;
    if (comparisionResult)
    {
        std::cout << "Error in enrollees comparision №7!" << std::endl;
        return 1;
    }

    return 0;
}

void InitSpecialty (UEAA::Specialty *specialty)
{
    specialty->SetIsPedagogical (true);
    specialty->AddAcceptedRODSubject (MATH_EXAM);

    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {MATH_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {PHYSICS_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (false, {BELARUSIAN_LANGUAGE_EXAM, RUSSIAN_LANGUAGE_EXAM}));
        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (MATH_EXAM);
        certPriority.push_back (PHYSICS_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
}
