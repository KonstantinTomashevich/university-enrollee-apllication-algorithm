#include <UEAA/BuildConfiguration.hpp>
#include "Enrollee.hpp"
#include <algorithm>
#include <cmath>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

namespace UEAA
{
Enrollee::Enrollee (const std::string &passportSeries, const std::string &passportNumber) :
    ReferenceCounted (),
    id_ (0),
    passportSeries_ (passportSeries),
    passportNumber_ (passportNumber),

    examsResults_ (),
    currentChoiceIndex_ (0),
    choices_ (0),
    lastUpdateResult_ (),

    certificateMarks_ (),
    certificateMedianMark_ (),
    hasSchoolGoldMedal_ (false),

    rodSubject_ (0),
    rodType_ (ROD_NONE)
{
    id_ = CalculateEnrolleeHash (this);
}

Enrollee::~Enrollee ()
{

}

unsigned Enrollee::GetId () const
{
    return id_;
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
    try
    {
        return examsResults_.at (examSubjectNameHash);
    }
    catch (std::out_of_range &exception)
    {
        return 0;
    }
}

void Enrollee::SetExamResult (unsigned examSubjectNameHash, unsigned char examResult)
{
    examsResults_ [examSubjectNameHash] = examResult;
}

bool Enrollee::HasMoreChoices () const
{
    return currentChoiceIndex_ < choices_.size ();
}

unsigned Enrollee::GetCurrentChoiceIndex () const
{
    return currentChoiceIndex_;
}

void Enrollee::IncreaseChoiceIndex ()
{
    currentChoiceIndex_++;
}

void Enrollee::RefreshChoiceIndex ()
{
    currentChoiceIndex_ = 0;
}

unsigned Enrollee::GetChoicesCount () const
{
    return choices_.size ();
}

const std::vector<EnrolleeChoice> &Enrollee::GetChoices () const
{
    return choices_;
}

EnrolleeChoice Enrollee::GetCurrentChoice () const
{
    if (HasMoreChoices ())
    {
        return choices_.at (currentChoiceIndex_);
    }
    else
    {
        return EMPTY_ENROLLEE_CHOICE;
    }
}

EnrolleeChoice Enrollee::GetChoiceByIndex (unsigned index) const
{
    if (index < choices_.size ())
    {
        return choices_.at (index);
    }
    else
    {
        return EMPTY_ENROLLEE_CHOICE;
    }
}

void Enrollee::AddChoiceToBack (const EnrolleeChoice &choice)
{
    choices_.push_back (choice);
}

void Enrollee::RemoveChoiceByIndex (unsigned index)
{
    choices_.erase (choices_.begin () + index);
}

void Enrollee::SwapChoicesAtIndexes (unsigned firstIndex, unsigned secondIndex)
{
    std::iter_swap (choices_.begin () + firstIndex, choices_.begin () + secondIndex);
}

EnrolleeChoice Enrollee::GetLastUpdateResult () const
{
    return lastUpdateResult_;
}

void Enrollee::SetLastUpdateResult (const EnrolleeChoice &lastUpdateResult)
{
    lastUpdateResult_ = lastUpdateResult;
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

bool Enrollee::HasSchoolGoldMedal () const
{
    return hasSchoolGoldMedal_;
}

void Enrollee::SetHasSchoolGoldMedal (bool hasSchoolGoldMedal)
{
    hasSchoolGoldMedal_ = hasSchoolGoldMedal;
}

unsigned Enrollee::GetRODSubject () const
{
    return rodSubject_;
}

void Enrollee::SetRODSubject (unsigned rodSubject)
{
    rodSubject_ = rodSubject;
}

RepublicanOlympiadDiplomaType Enrollee::GetRODType () const
{
    return rodType_;
}

void Enrollee::SetRODType (RepublicanOlympiadDiplomaType rodType)
{
    rodType_ = rodType;
}
}
