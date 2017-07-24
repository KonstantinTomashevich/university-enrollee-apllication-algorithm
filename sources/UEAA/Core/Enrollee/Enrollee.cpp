#include "Enrollee.hpp"
#include <algorithm>
#include <math.h>

namespace UEAA
{
Enrollee::Enrollee (const std::string &passportSeries, const std::string &passportNumber) :
    ReferenceCounted (),
    passportSeries_ (passportSeries),
    passportNumber_ (passportNumber),

    examsResults_ (),
    currentChoiseIndex_ (0),
    choises_ (0),

    certificateMarks_ (),
    certificateMedianMark_ ()
{

}

Enrollee::~Enrollee ()
{

}

const std::string &Enrollee::GetPassportSeries () const
{
    return passportSeries_;
}

const std::string &Enrollee::GetPassportNumber () const
{
    return passportNumber_;
}

unsigned char Enrollee::GetExamResult (unsigned examSubjectNameHash) const
{
    return examsResults_.at (examSubjectNameHash);
}

void Enrollee::SetExamResult (unsigned examSubjectNameHash, unsigned char examResult)
{
    examsResults_ [examSubjectNameHash] = examResult;
}

bool Enrollee::HasMoreChoises () const
{
    return currentChoiseIndex_ < choises_.size ();
}

unsigned Enrollee::GetCurrentChoiseIndex () const
{
    return currentChoiseIndex_;
}

void Enrollee::IncreaseChoiceIndex ()
{
    currentChoiseIndex_++;
}

void Enrollee::RefreshChoiseIndex ()
{
    currentChoiseIndex_ = 0;
}

unsigned Enrollee::GetChoisesCount () const
{
    return choises_.size ();
}

const std::vector<EnrolleeChoise> &Enrollee::GetChoices () const
{
    return choises_;
}

EnrolleeChoise Enrollee::GetCurrentChoise () const
{
    if (HasMoreChoises ())
    {
        return choises_ [currentChoiseIndex_];
    }
    else
    {
        return EMPTY_ENROLLEE_CHOISE;
    }
}

EnrolleeChoise Enrollee::GetChoiseByIndex (unsigned index) const
{
    if (index < choises_.size ())
    {
        return choises_ [index];
    }
    else
    {
        return EMPTY_ENROLLEE_CHOISE;
    }
}

void Enrollee::AddChoiseToBack (const EnrolleeChoise &choise)
{
    choises_.push_back (choise);
}

void Enrollee::RemoveChoiseByIndex (unsigned index)
{
    choises_.erase (choises_.begin () + index);
}

void Enrollee::SwapChoisesAtIndexes (unsigned firstIndex, unsigned secondIndex)
{
    std::iter_swap (choises_.begin () + firstIndex, choises_.begin () + secondIndex);
}

unsigned char Enrollee::GetCertificateMark (unsigned subjectNameHash) const
{
    return certificateMarks_.at (subjectNameHash);
}

void Enrollee::SetCertificateMark (unsigned subjectNameHash, unsigned char mark)
{
    certificateMarks_ [subjectNameHash] = mark;
}

float Enrollee::GetCertificateMedianMark () const
{
    return certificateMedianMark_;
}

void Enrollee::CalculateCertificateMedianMark ()
{
    int sum = 0;
    for (auto iterator = certificateMarks_.cbegin (); iterator != certificateMarks_.cend (); iterator++)
    {
        sum += iterator->second;
    }

    certificateMedianMark_ = std::round (sum * 10.0f / certificateMarks_.size ()) / 10.0f;
}
}
