#include <UEAA/BuildConfiguration.hpp>
#include "EnrolleeHelpers.hpp"
#include <cmath>
#include <UEAA/Utils/CStringToHash.hpp>

namespace UEAA
{
namespace IsFirstEnrolleeBetterSteps
{
/// Returns true if comparision finished at this step.
bool CheckIsBothCanChoiceSpecialty (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
bool CheckRODs (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
bool CheckSchoolGoldMedals (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
bool CheckScores (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
bool CheckPrimaryExams (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
bool CheckSchoolCertificateMarks (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput);
}

bool IsFirstEnrolleeBetter (const Specialty *specialty, const Enrollee *first, const Enrollee *second)
{
    // TODO: Not all BSU checks implemented.
    bool result = false;
    if (IsFirstEnrolleeBetterSteps::CheckIsBothCanChoiceSpecialty (specialty, first, second, result) ||
        IsFirstEnrolleeBetterSteps::CheckRODs (specialty, first, second, result) ||
        IsFirstEnrolleeBetterSteps::CheckSchoolGoldMedals (specialty, first, second, result) ||
        IsFirstEnrolleeBetterSteps::CheckScores (specialty, first, second, result) ||
        IsFirstEnrolleeBetterSteps::CheckPrimaryExams (specialty, first, second, result) ||
        IsFirstEnrolleeBetterSteps::CheckSchoolCertificateMarks (specialty, first, second, result))
    {
        return result;
    }
    else
    {
        return false;
    }
}

namespace IsFirstEnrolleeBetterSteps
{
bool CheckIsBothCanChoiceSpecialty (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    if (!CanEnrolleeChoiceSpecialty (specialty, first))
    {
        resultOutput = false;
        return true;
    }
    else if (!CanEnrolleeChoiceSpecialty (specialty, second))
    {
        resultOutput = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool CheckRODs (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    if (first->GetRODType () != ROD_NONE && second->GetRODType () != ROD_NONE)
    {
        if (specialty->IsRODSubjectAccepted (first->GetRODSubject ()) &&
            specialty->IsRODSubjectAccepted (second->GetRODSubject ()))
        {
            resultOutput = IsFirstDiplomaBetter (first->GetRODType (), second->GetRODType ());
            return true;
        }
        else if (specialty->IsRODSubjectAccepted (first->GetRODSubject ()))
        {
            resultOutput = true;
            return true;
        }
        else if (specialty->IsRODSubjectAccepted (second->GetRODSubject ()) )
        {
            resultOutput = false;
            return true;
        }
    }

    else if (first->GetRODType () != ROD_NONE && specialty->IsRODSubjectAccepted (first->GetRODSubject ()))
    {
        resultOutput = true;
        return true;
    }

    else if (second->GetRODType () != ROD_NONE && specialty->IsRODSubjectAccepted (second->GetRODSubject ()))
    {
        resultOutput = false;
        return true;
    }

    else
    {
        return false;
    }
}

bool CheckSchoolGoldMedals (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    if (specialty->IsPedagogical ())
    {
        if (first->HasSchoolGoldMedal () && !second->HasSchoolGoldMedal ())
        {
            resultOutput = true;
            return true;
        }
        else if (!first->HasSchoolGoldMedal () && second->HasSchoolGoldMedal ())
        {
            resultOutput = false;
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool CheckScores (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    unsigned firstScore = CalculateEnrolleeScore (specialty, first);
    unsigned secondScore = CalculateEnrolleeScore (specialty, second);

    if (firstScore != secondScore)
    {
        resultOutput = firstScore > secondScore;
        return true;
    }
    else
    {
        return false;
    }
}

bool CheckPrimaryExams (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    const std::vector <std::pair <bool, std::vector <unsigned> > > &exams = specialty->GetRequiredExams ();
    for (auto iterator = exams.cbegin (); iterator != exams.cend (); iterator++)
    {
        if (iterator->first)
        {
            unsigned char firstEnrolleeResult = GetEnrolleeBestResultFromExams (first,  iterator->second);
            unsigned char secondEnrolleeResult = GetEnrolleeBestResultFromExams (second, iterator->second);

            if (firstEnrolleeResult != secondEnrolleeResult)
            {
                resultOutput = firstEnrolleeResult > secondEnrolleeResult;
                return true;
            }
        }
    }
    return false;
}

bool CheckSchoolCertificateMarks (const Specialty *specialty, const Enrollee *first, const Enrollee *second, bool &resultOutput)
{
    const std::vector <unsigned> &marksPriority = specialty->GetMarksInCertificatePriority ();
    for (auto iterator = marksPriority.cbegin (); iterator != marksPriority.cend (); iterator++)
    {
        unsigned char firstMark = first->GetCertificateMark (*iterator);
        unsigned char secondMark = second->GetCertificateMark (*iterator);

        if (firstMark != secondMark)
        {
            resultOutput = firstMark > secondMark;
            return true;
        }
    }
    return false;
}
}

bool CanEnrolleeChoiceSpecialty (const Specialty *specialty, const Enrollee *enrollee)
{
    if (enrollee->GetRODType () != ROD_NONE && specialty->IsRODSubjectAccepted (enrollee->GetRODSubject ()))
    {
        return true;
    }
    else
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

unsigned char GetEnrolleeBestResultFromExams (const Enrollee *enrollee, const std::vector <unsigned> &exams)
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
}
