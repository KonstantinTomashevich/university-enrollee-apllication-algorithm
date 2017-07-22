#include <UniversityEnrolleeApllicationAlgorithm/Core/Enrollee.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Utils/SharedPointer.hpp>
#include <UniversityEnrolleeApllicationAlgorithm/Utils/CStringToHash.hpp>
#include <iostream>

const unsigned MATH_RESULT = 93;
const unsigned PHYSICS_RESULT = 92;
const unsigned LANGUAGE_RESULT = 97;

const char *ERROR_INCORRECT_EXAMS_RESULTS = "Incorrect exams results!";
const char *ERROR_INCORRECT_CHOISES_COUNT = "Incorrect choises count!";
const char *ERROR_INCORRECT_CHOISES_COUNT_AFTER_FIRST_REMOVE = "Incorrect choises count after first remove!";
const char *ERROR_INCORRECT_CHOISE_FACULTY = "Incorrect choise faculty!";
const char *ERROR_INCORRECT_HAS_MORE_CHOICES = "Incorrect has more choises return!";
const char *ERROR_INCORRECT_CHOISE_AFTER_REFRESH = "Incorrect choice after refresh!";

using namespace UniversityEnrolleeApllicationAlgorithm;
const unsigned MATH_EXAM = CStringToHash ("Math");
const unsigned PHYSICS_EXAM = CStringToHash ("Physics");
const unsigned LANGUAGE_EXAM = CStringToHash ("Language");

int main ()
{
    SharedPointer <Enrollee> enrollee (new Enrollee ());
    enrollee->SetPasportSeries ("XX");
    enrollee->SetPasportNumber ("0000000");
    std::cout << "Setting exams results: " << MATH_RESULT << " " <<
                 PHYSICS_RESULT << " " << LANGUAGE_RESULT << std::endl;

    enrollee->SetExamResult (MATH_EXAM, MATH_RESULT);
    enrollee->SetExamResult (PHYSICS_EXAM, PHYSICS_RESULT);
    enrollee->SetExamResult (LANGUAGE_EXAM, LANGUAGE_RESULT);

    std::cout << "Setted exams results: " << enrollee->GetExamResult (MATH_EXAM) << " " <<
                 enrollee->GetExamResult (PHYSICS_EXAM) << " " << enrollee->GetExamResult (LANGUAGE_EXAM)<< std::endl;

    if (enrollee->GetExamResult (MATH_EXAM) != MATH_RESULT ||
            enrollee->GetExamResult (PHYSICS_EXAM) != PHYSICS_RESULT ||
            enrollee->GetExamResult (LANGUAGE_EXAM) != LANGUAGE_RESULT)
    {
        std::cout << ERROR_INCORRECT_EXAMS_RESULTS << std::endl;
        return 1;
    }

    for (unsigned faculty = 0; faculty <= 3; faculty++)
    {
        EnrolleeChoise choise (faculty, 1, STUDY_FORM_FREE);
        std::cout << "Added faculty choise: " << choise.faculty_ << std::endl;
        enrollee->AddChoiseToBack (choise);
    }

    std::cout << "Choises count: " << enrollee->GetChoisesCount () << std::endl;
    if (enrollee->GetChoisesCount () != 4)
    {
        std::cout << ERROR_INCORRECT_CHOISES_COUNT << std::endl;
        return 1;
    }

    enrollee->RemoveChoiseByIndex (0);
    std::cout << "Choises count after first remove: " << enrollee->GetChoisesCount () << std::endl;
    if (enrollee->GetChoisesCount () != 3)
    {
        std::cout << ERROR_INCORRECT_CHOISES_COUNT_AFTER_FIRST_REMOVE << std::endl;
        return 1;
    }

    enrollee->SwapChoisesAtIndexes (0, 2);
    std::cout << "Choises 0 and 2 swapped." << std::endl;
    for (unsigned faculty = 3; faculty >= 1; faculty--)
    {
        EnrolleeChoise choise = enrollee->GetCurrentChoise ();
        std::cout << "Choise (faculty): " << choise.faculty_ << std::endl;
        if (choise.faculty_ != faculty)
        {
            std::cout << ERROR_INCORRECT_CHOISE_FACULTY<< std::endl;
            return 1;
        }
        enrollee->IncreaseChoiceIndex ();
    }

    std::cout << "Has more choices: " << enrollee->HasMoreChoises () << std::endl;
    if (enrollee->HasMoreChoises ())
    {
        std::cout << ERROR_INCORRECT_HAS_MORE_CHOICES << std::endl;
        return 1;
    }

    enrollee->RefreshChoiseIndex ();
    std::cout << "Choise index refreshed." << std::endl;
    std::cout << "Current choise: (faculty): " << enrollee->GetCurrentChoise ().faculty_ << std::endl;
    if (enrollee->GetCurrentChoise ().faculty_ != enrollee->GetChoiseByIndex (0).faculty_)
    {
        std::cout << ERROR_INCORRECT_CHOISE_AFTER_REFRESH << std::endl;
        return 1;
    }

    return 0;
}
