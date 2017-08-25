#include <UEAA/BuildConfiguration.hpp>
#include "Enrollee.hpp"
#include <algorithm>
#include <cmath>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

namespace UEAA
{
Enrollee::Enrollee (unsigned id) :
    XMLSerializable (),
    id_ (id),

    examsResults_ (),
    currentChoiceIndex_ (0),
    choices_ (0),
    lastUpdateResult_ (nullptr),

    certificateMarks_ (),
    certificateMedianMark_ (),
    hasSchoolGoldMedal_ (false),

    rodSubject_ (0),
    rodType_ (ROD_NONE)
{

}

Enrollee::~Enrollee ()
{

}

unsigned Enrollee::GetId () const
{
    return id_;
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

const std::list <SharedPointer <EnrolleeChoice> > &Enrollee::GetChoices () const
{
    return choices_;
}

EnrolleeChoice *Enrollee::GetCurrentChoice () const
{
    return GetChoiceByIndex (currentChoiceIndex_);
}

EnrolleeChoice *Enrollee::GetChoiceByIndex (unsigned index) const
{
    if (index < choices_.size ())
    {
        auto iterator = choices_.begin ();
        for (; index > 0; iterator++, index--) {}
        return *iterator;
    }
    else
    {
        return nullptr;
    }
}

void Enrollee::AddChoiceToBack (EnrolleeChoice *choice)
{
    choices_.push_back (SharedPointer <EnrolleeChoice> (choice));
}

void Enrollee::RemoveChoiceByIndex (unsigned index)
{
    if (index < choices_.size ())
    {
        auto iterator = choices_.begin ();
        for (; index > 0; iterator++, index--) {}
        choices_.erase (iterator);
    }
}

void Enrollee::SwapChoicesAtIndexes (unsigned firstIndex, unsigned secondIndex)
{
    auto firstIterator = choices_.begin ();
    for (; firstIndex > 0; firstIterator++, firstIndex--) {}

    auto secondIterator = choices_.begin ();
    for (; secondIndex > 0; secondIterator++, secondIndex--) {}

    SharedPointer <EnrolleeChoice> temp (firstIterator->GetTrackingObject ());
    firstIterator->SetTrackingObject (secondIterator->GetTrackingObject ());
    secondIterator->SetTrackingObject (temp.GetTrackingObject ());
}

EnrolleeChoice *Enrollee::GetLastUpdateResult () const
{
    return lastUpdateResult_;
}

void Enrollee::SetLastUpdateResult (EnrolleeChoice *lastUpdateResult)
{
    lastUpdateResult_.SetTrackingObject (lastUpdateResult);
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

void Enrollee::CheckIsHasSchoolGoldMedal ()
{
    hasSchoolGoldMedal_ = true;
    for (auto iterator = certificateMarks_.cbegin (); iterator != certificateMarks_.cend (); iterator++)
    {
        if (iterator->second < 9)
        {
            hasSchoolGoldMedal_ = false;
            return;
        }
    }
}

unsigned Enrollee::GetRODSubject () const
{
    return rodSubject_;
}

void Enrollee::SetRODSubject (unsigned rodSubject)
{
    rodSubject_ = rodSubject;
}

RODType Enrollee::GetRODType () const
{
    return rodType_;
}

void Enrollee::SetRODType (RODType rodType)
{
    rodType_ = rodType;
}

void Enrollee::SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetAttribute ("passport", deHashTable->DeHash (id_).c_str ());
    output->SetAttribute ("rodSubject", deHashTable->DeHash (rodSubject_).c_str ());
    output->SetAttribute ("rodType", rodType_);

    tinyxml2::XMLElement *examsResultsElement = document.NewElement ("examsResults");
    output->InsertEndChild (examsResultsElement);

    for (auto iterator = examsResults_.cbegin (); iterator != examsResults_.cend (); iterator++)
    {
        tinyxml2::XMLElement *examElement = document.NewElement ("exam");
        examsResultsElement->InsertEndChild (examElement);

        examElement->SetAttribute ("subject", deHashTable->DeHash (iterator->first).c_str ());
        examElement->SetAttribute ("result", iterator->second);
    }

    tinyxml2::XMLElement *certificateMarksElement = document.NewElement ("certificateMarks");
    output->InsertEndChild (certificateMarksElement);

    for (auto iterator = certificateMarks_.cbegin (); iterator != certificateMarks_.cend (); iterator++)
    {
        tinyxml2::XMLElement *markElement = document.NewElement ("mark");
        certificateMarksElement->InsertEndChild (markElement);

        markElement->SetAttribute ("subject", deHashTable->DeHash (iterator->first).c_str ());
        markElement->SetAttribute ("result", iterator->second);
    }

    tinyxml2::XMLElement *choicesElement = document.NewElement ("choices");
    output->InsertEndChild (choicesElement);

    for (auto iterator = choices_.cbegin (); iterator != choices_.cend (); iterator++)
    {
        EnrolleeChoice *choice = *iterator;
        tinyxml2::XMLElement *choiceElement = document.NewElement ("choice");

        choicesElement->InsertEndChild (choiceElement);
        choice->SaveToXML (document, choiceElement, deHashTable);
    }

    if (lastUpdateResult_.GetTrackingObject () != nullptr)
    {
        tinyxml2::XMLElement *lastUpdateResultElement = document.NewElement ("lastUpdateResult");
        output->InsertEndChild (lastUpdateResultElement);
        lastUpdateResult_->SaveToXML (document, lastUpdateResultElement, deHashTable);
    }
}

void Enrollee::LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable)
{

}

bool Enrollee::operator == (const Enrollee &rhs) const
{
    if (id_ != rhs.id_ ||
        examsResults_ != rhs.examsResults_ ||
        currentChoiceIndex_ != rhs.currentChoiceIndex_ ||
        certificateMarks_ != rhs.certificateMarks_ ||
        certificateMedianMark_ != rhs.certificateMedianMark_ ||
        hasSchoolGoldMedal_ != rhs.hasSchoolGoldMedal_ ||
        rodSubject_ != rhs.rodSubject_ ||
        rodType_ != rhs.rodType_)
    {
        return false;
    }

    if (choices_.size () == rhs.choices_.size ())
    {
        auto firstIterator = choices_.cbegin ();
        auto secondIterator = rhs.choices_.cbegin ();

        for (; firstIterator != choices_.cend () && secondIterator != rhs.choices_.cend ();
               firstIterator++, secondIterator++)
        {
            if (*firstIterator->GetTrackingObject () != *secondIterator->GetTrackingObject ())
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    if (lastUpdateResult_.GetTrackingObject () != nullptr && rhs.lastUpdateResult_.GetTrackingObject () != nullptr)
    {
        return *lastUpdateResult_.GetTrackingObject () == *lastUpdateResult_.GetTrackingObject ();
    }
    else
    {
        return lastUpdateResult_.GetTrackingObject () == rhs.lastUpdateResult_.GetTrackingObject ();
    }
}

bool Enrollee::operator != (const Enrollee &rhs) const
{
    return !(rhs == *this);
}
}
