#include <UEAA/BuildConfiguration.hpp>
#include "EnrolleeHelpers.hpp"
#include <cmath>
#include <UEAA/Utils/CStringToHash.hpp>

namespace UEAA
{
bool IsFirstEnrolleeBetter (const Specialty *specialty, const Enrollee *first, const Enrollee *second)
{
    // TODO: Maybe split this function to smaller pieces?
    if (!CanEnrolleeChoiceSpecialty (specialty, first))
    {
        return false;
    }
    else if (!CanEnrolleeChoiceSpecialty (specialty, second))
    {
        return true;
    }

    if (first->GetRODType () != ROD_NONE && second->GetRODType () != ROD_NONE)
    {
        if (specialty->IsRODSubjectAccepted (first->GetRODSubject ()) &&
                specialty->IsRODSubjectAccepted (second->GetRODSubject ()))
        {
            return IsFirstDiplomaBetter (first->GetRODType (), second->GetRODType ());
        }
        else if (specialty->IsRODSubjectAccepted (first->GetRODSubject ()))
        {
            return true;
        }
        else if (specialty->IsRODSubjectAccepted (second->GetRODSubject ()) )
        {
            return false;
        }
    }

    else if (first->GetRODType () != ROD_NONE && specialty->IsRODSubjectAccepted (first->GetRODSubject ()))
    {
        return true;
    }

    else if (second->GetRODType () != ROD_NONE && specialty->IsRODSubjectAccepted (second->GetRODSubject ()))
    {
        return false;
    }


    if (specialty->IsPedagogical ())
    {
        if (first->HasSchoolGoldMedal () && !second->HasSchoolGoldMedal ())
        {
            return true;
        }
        else if (!first->HasSchoolGoldMedal () && second->HasSchoolGoldMedal ())
        {
            return false;
        }
    }

    unsigned firstScore = CalculateEnrolleeScore (specialty, first);
    unsigned secondScore = CalculateEnrolleeScore (specialty, second);

    if (firstScore != secondScore)
    {
        return firstScore > secondScore;
    }

    const std::vector <std::pair <bool, std::vector <unsigned> > > &exams = specialty->GetRequiredExams ();
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        if (iterator->first)
        {
            unsigned char firstEnrolleeResult = GetEnrolleeBestResultFromExams (first,  iterator->second);
            unsigned char secondEnrolleeResult = GetEnrolleeBestResultFromExams (second, iterator->second);

            if (firstEnrolleeResult != secondEnrolleeResult)
            {
                return firstEnrolleeResult > secondEnrolleeResult;
            }
        }
    }

    const std::vector <unsigned> &marksPriority = specialty->GetMarksInCertificatePriority ();
    for (auto iterator = marksPriority.cbegin (); iterator != marksPriority.cend (); iterator++)
    {
        unsigned char firstMark = first->GetCertificateMark (*iterator);
        unsigned char secondMark = second->GetCertificateMark (*iterator);

        if (firstMark != secondMark)
        {
            return firstMark > secondMark;
        }
    }

    // TODO: Not all BSU checks implemented.
    return false;
}

bool CanEnrolleeChoiceSpecialty (const Specialty *specialty, const Enrollee *enrollee)
{
    const std::vector <std::pair <bool, std::vector <unsigned> > > &exams = specialty->GetRequiredExams ();
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        if (GetEnrolleeBestResultFromExams (enrollee, iterator->second) == 0)
        {
            return false;
        }
    }
    return true;
}

unsigned CalculateEnrolleeScore (const Specialty *specialty, const Enrollee *enrollee)
{
    const std::vector <std::pair <bool, std::vector <unsigned> > > &exams = specialty->GetRequiredExams ();
    unsigned score = std::round (enrollee->GetCertificateMedianMark () * 10);

    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        score += GetEnrolleeBestResultFromExams (enrollee, iterator->second);
    }
    return score;
}

unsigned GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams)
{
    unsigned char best = 0;
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        unsigned char result = enrollee->GetExamResult (*iterator);
        if (result > best)
        {
            best = result;
        }
    }
    return best;
}

unsigned CalculateEnrolleeHash (const Enrollee *enrollee, DeHashTable *deHashTable)
{
    return CalculateEnrolleeHash (enrollee->GetPassportSeries (), enrollee->GetPassportNumber (), deHashTable);
}

unsigned CalculateEnrolleeHash (const std::string &passportSeries, const std::string &passportNumber, DeHashTable *deHashTable)
{
    return CStringToHash ((passportSeries + passportNumber).c_str (), deHashTable);
}
}
