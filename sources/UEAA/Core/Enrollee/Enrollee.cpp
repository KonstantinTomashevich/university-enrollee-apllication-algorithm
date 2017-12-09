#include <UEAA/BuildConfiguration.hpp>
#include "Enrollee.hpp"
#include <algorithm>
#include <cmath>

#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>
#include <UEAA/Utils/CStringToHash.hpp>

namespace UEAA
{
Enrollee::Enrollee (unsigned id) :
    XMLSerializable (),
    id_ (id),

    examsResults_ (),
    choices_ (0),
    currentChoice_ (choices_.begin ()),
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

std::vector <unsigned> Enrollee::GetPassedExamsSubjects () const
{
    std::vector <unsigned> subjects;
    for (auto iterator = examsResults_.begin (); iterator != examsResults_.end (); iterator++)
    {
        subjects.push_back (iterator->first);
    }
    return subjects;
}

bool Enrollee::HasMoreChoices () const
{
    return !choices_.empty () && currentChoice_ != choices_.end ();
}

void Enrollee::StepToNextChoice ()
{
    if (HasMoreChoices ())
    {
        currentChoice_++;
    }
}

void Enrollee::RefreshCurrentChoice ()
{
    currentChoice_ = choices_.begin ();
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
    if (HasMoreChoices ())
    {
        return *currentChoice_;
    }
    else
    {
        return nullptr;
    }
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
    bool first = choices_.empty ();
    choices_.emplace_back (SharedPointer <EnrolleeChoice> (choice));

    if (first)
    {
        currentChoice_ = choices_.begin ();
    }
}

void Enrollee::RemoveChoiceByIndex (unsigned index)
{
    if (index < choices_.size ())
    {
        auto iterator = choices_.begin ();
        for (; index > 0; iterator++, index--) {}

        if (iterator == currentChoice_)
        {
            currentChoice_ = choices_.erase (iterator);
        }
        else
        {
            choices_.erase (iterator);
        }
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

std::vector <unsigned> Enrollee::GetCertificateMarksSubjects () const
{
    std::vector <unsigned> subjects;
    for (auto iterator = certificateMarks_.cbegin (); iterator != certificateMarks_.cend (); iterator++)
    {
        subjects.push_back (iterator->first);
    }
    return subjects;
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
    Clear ();
    rodSubject_ = CStringToHash (input->Attribute ("rodSubject"), deHashTable);
    rodType_ = static_cast <RODType> (atoi (input->Attribute ("rodType")));

    tinyxml2::XMLElement *examsResultsElement = input->FirstChildElement ("examsResults");
    if (examsResultsElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = examsResultsElement->FirstChildElement ("exam");
                element != nullptr; element = element->NextSiblingElement ("exam"))
        {
            unsigned subject = CStringToHash (element->Attribute ("subject"), deHashTable);
            unsigned result = 0;
            tinyxml2::XMLUtil::ToUnsigned (element->Attribute ("result"), &result);
            examsResults_ [subject] = static_cast <unsigned char> (result);
        }
    }

    tinyxml2::XMLElement *certificateMarksElement = input->FirstChildElement ("certificateMarks");
    if (certificateMarksElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = certificateMarksElement->FirstChildElement ("mark");
             element != nullptr; element = element->NextSiblingElement ("mark"))
        {
            unsigned subject = CStringToHash (element->Attribute ("subject"), deHashTable);
            unsigned result = 0;
            tinyxml2::XMLUtil::ToUnsigned (element->Attribute ("result"), &result);
            certificateMarks_ [subject] = static_cast <unsigned char> (result);
        }
    }
    
    CalculateCertificateMedianMark ();
    CheckIsHasSchoolGoldMedal ();

    tinyxml2::XMLElement *choicesElement = input->FirstChildElement ("choices");
    if (choicesElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = choicesElement->FirstChildElement ("choice");
             element != nullptr; element = element->NextSiblingElement ("choice"))
        {
            SharedPointer <EnrolleeChoice> choice (new EnrolleeChoice ());
            choice->LoadFromXML (element, deHashTable);
            AddChoiceToBack (choice);
        }
    }

    tinyxml2::XMLElement *lastUpdateResultElement = input->FirstChildElement ("lastUpdateResult");
    if (lastUpdateResultElement != nullptr)
    {
        lastUpdateResult_.SetTrackingObject (new EnrolleeChoice ());
        lastUpdateResult_->LoadFromXML (lastUpdateResultElement, deHashTable);
    }
}

void Enrollee::Clear ()
{
    examsResults_.clear ();
    choices_.clear ();
    RefreshCurrentChoice ();

    lastUpdateResult_.SetTrackingObject (nullptr);
    certificateMarks_.clear ();
    certificateMedianMark_ = 0.0f;

    hasSchoolGoldMedal_ = false;
    rodSubject_ = 0;
    rodType_ = ROD_NONE;
}

bool Enrollee::operator == (const Enrollee &rhs) const
{
    if (id_ != rhs.id_ ||
        examsResults_ != rhs.examsResults_ ||
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
