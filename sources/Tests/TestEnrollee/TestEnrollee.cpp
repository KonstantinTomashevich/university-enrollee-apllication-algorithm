#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <iostream>
#include <cmath>

const unsigned char MATH_RESULT = 93;
const unsigned char PHYSICS_RESULT = 92;
const unsigned char LANGUAGE_RESULT = 97;

const unsigned char MATH_MARK = 9;
const unsigned char PHYSICS_MARK = 9;
const unsigned char LANGUAGE_MARK = 8;

const unsigned MATH_EXAM = UEAA::CStringToHash ("Math");
const unsigned PHYSICS_EXAM = UEAA::CStringToHash ("Physics");
const unsigned LANGUAGE_EXAM = UEAA::CStringToHash ("Language");

int main ()
{
    UEAA::SharedPointer <UEAA::Enrollee> enrollee (new UEAA::Enrollee ("XX", "0000000"));
    std::cout << "Setting exams results: " << MATH_RESULT << " " <<
                 PHYSICS_RESULT << " " << LANGUAGE_RESULT << std::endl;

    enrollee->SetExamResult (MATH_EXAM, MATH_RESULT);
    enrollee->SetExamResult (PHYSICS_EXAM, PHYSICS_RESULT);
    enrollee->SetExamResult (LANGUAGE_EXAM, LANGUAGE_RESULT);

    std::cout << "Setted exams results: " << enrollee->GetExamResult (MATH_EXAM) * 1 << " " <<
                 enrollee->GetExamResult (PHYSICS_EXAM) * 1 << " " << enrollee->GetExamResult (LANGUAGE_EXAM) * 1 << std::endl;

    if (enrollee->GetExamResult (MATH_EXAM) != MATH_RESULT ||
            enrollee->GetExamResult (PHYSICS_EXAM) != PHYSICS_RESULT ||
            enrollee->GetExamResult (LANGUAGE_EXAM) != LANGUAGE_RESULT)
    {
        std::cout << "Incorrect exams results!" << std::endl;
        return 1;
    }

    enrollee->SetCertificateMark (MATH_EXAM, MATH_MARK);
    enrollee->SetCertificateMark (PHYSICS_EXAM, PHYSICS_MARK);
    enrollee->SetCertificateMark (LANGUAGE_EXAM, LANGUAGE_MARK);
    enrollee->CalculateCertificateMedianMark ();

    std::cout << "Certificate median mark: " << enrollee->GetCertificateMedianMark () << std::endl;
    if (enrollee->GetCertificateMedianMark () != 8.7f)
    {
        std::cout << "Incorrect median mark!" << std::endl;
        return 1;
    }

    for (unsigned faculty = 0; faculty <= 3; faculty++)
    {
        UEAA::EnrolleeChoice choice (faculty, 1, UEAA::STUDY_FORM_FREE);
        std::cout << "Added faculty choice: " << choice.faculty_ << std::endl;
        enrollee->AddChoiceToBack (choice);
    }

    std::cout << "Choices count: " << enrollee->GetChoicesCount () << std::endl;
    if (enrollee->GetChoicesCount () != 4)
    {
        std::cout << "Incorrect choices count!" << std::endl;
        return 1;
    }

    enrollee->RemoveChoiceByIndex (0);
    std::cout << "Choices count after first remove: " << enrollee->GetChoicesCount () << std::endl;
    if (enrollee->GetChoicesCount () != 3)
    {
        std::cout << "Incorrect choices count after first remove!" << std::endl;
        return 1;
    }

    enrollee->SwapChoicesAtIndexes (0, 2);
    std::cout << "Choices 0 and 2 swapped." << std::endl;
    for (unsigned faculty = 3; faculty >= 1; faculty--)
    {
        UEAA::EnrolleeChoice choice = enrollee->GetCurrentChoice ();
        std::cout << "Choice (faculty): " << choice.faculty_ << std::endl;
        if (choice.faculty_ != faculty)
        {
            std::cout << "Incorrect choice faculty!"<< std::endl;
            return 1;
        }
        enrollee->IncreaseChoiceIndex ();
    }

    std::cout << "Has more choices: " << enrollee->HasMoreChoices () << std::endl;
    if (enrollee->HasMoreChoices ())
    {
        std::cout << "Incorrect has more choices return!" << std::endl;
        return 1;
    }

    enrollee->RefreshChoiceIndex ();
    std::cout << "Choice index refreshed." << std::endl;
    std::cout << "Current choice: (faculty): " << enrollee->GetCurrentChoice ().faculty_ << std::endl;
    if (enrollee->GetCurrentChoice ().faculty_ != enrollee->GetChoiceByIndex (0).faculty_)
    {
        std::cout << "Incorrect choice after refresh!" << std::endl;
        return 1;
    }

    return 0;
}
