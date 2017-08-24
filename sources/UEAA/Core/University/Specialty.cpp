#include <UEAA/BuildConfiguration.hpp>
#include "Specialty.hpp"
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>
#include <algorithm>

#include <iostream>

namespace UEAA
{
Specialty::Specialty (Faculty *parent, unsigned id) :
    XMLSerializable (),
    parent_ (parent),
    id_ (id),

    requiredExams_ (),
    marksInCertificatePriority_ (),

    enrolleesInFreeForm_ (),
    enrolleesInPaidForm_ (),

    maxEnrolleesInFreeForm_ (0),
    maxEnrolleesInPaidForm_ (0),

    isPedagogical_ (false),
    acceptedRODSubjects_ ()
{

}

Specialty::~Specialty ()
{

}

unsigned Specialty::GetId () const
{
    return id_;
}

Faculty *Specialty::GetParent () const
{
    return parent_;
}

const std::vector <std::pair <bool, std::vector <unsigned> > > &Specialty::GetRequiredExams () const
{
    return requiredExams_;
}

void Specialty::SetRequiredExams (const std::vector <std::pair <bool, std::vector <unsigned> > > &requiredExams)
{
    requiredExams_ = requiredExams;
}

const std::vector <unsigned> &Specialty::GetMarksInCertificatePriority () const
{
    return marksInCertificatePriority_;
}

void Specialty::SetMarksInCertificatePriority (const std::vector <unsigned> &marksInCertificatePriority)
{
    marksInCertificatePriority_ = marksInCertificatePriority;
}

const std::vector <Enrollee *> &Specialty::GetEnrolleesInFreeForm () const
{
    return enrolleesInFreeForm_;
}

const std::vector <Enrollee *> &Specialty::GetEnrolleesInPaidForm () const
{
    return enrolleesInPaidForm_;
}

bool Specialty::AddEnrollee (Enrollee *enrollee)
{
    EnrolleeChoice choice = enrollee->GetCurrentChoice ();
    if (choice.specialty_ != id_)
    {
        return false;
    }

    if (!CanEnrolleeChoiceSpecialty (this, enrollee))
    {
        return false;
    }

    AddEnrolleeToOrder (enrollee, (choice.studyForm_ == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_);
    return true;
}

std::vector <Enrollee *> Specialty::RemoveExcessEnrollees ()
{
    std::vector <Enrollee *> excessEnrollees;
    bool freeChanged = GetExcessEnrollees (excessEnrollees, STUDY_FORM_FREE);
    bool paidChanged = GetExcessEnrollees (excessEnrollees, STUDY_FORM_PAID);

    if (freeChanged)
    {
        enrolleesInFreeForm_.erase (enrolleesInFreeForm_.begin () + maxEnrolleesInFreeForm_, enrolleesInFreeForm_.end ());
        enrolleesInFreeForm_.shrink_to_fit ();
    }

    if (paidChanged)
    {
        enrolleesInPaidForm_.erase (enrolleesInPaidForm_.begin () + maxEnrolleesInPaidForm_, enrolleesInPaidForm_.end ());
        enrolleesInPaidForm_.shrink_to_fit ();
    }
    return excessEnrollees;
}

void Specialty::ClearEnrollees ()
{
    enrolleesInFreeForm_.clear ();
    enrolleesInPaidForm_.clear ();
}

unsigned Specialty::GetMaxEnrolleesInFreeForm () const
{
    return maxEnrolleesInFreeForm_;
}

void Specialty::SetMaxEnrolleesInFreeForm (unsigned maxEnrolleesInFreeForm)
{
    maxEnrolleesInFreeForm_ = maxEnrolleesInFreeForm;
}

unsigned Specialty::GetMaxEnrolleesInPaidForm () const
{
    return maxEnrolleesInPaidForm_;
}

void Specialty::SetMaxEnrolleesInPaidForm (unsigned maxEnrolleesInPaidForm)
{
    maxEnrolleesInPaidForm_ = maxEnrolleesInPaidForm;
}

bool Specialty::GetExcessEnrollees (std::vector <Enrollee *> &output, StudyForm studyForm) const
{
    const std::vector <Enrollee *> &enrollees = (studyForm == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_;
    unsigned maxEnrollees = (studyForm == STUDY_FORM_FREE) ? maxEnrolleesInFreeForm_ : maxEnrolleesInPaidForm_;
    bool changed = false;

    if (enrollees.size () > maxEnrollees)
    {
        for (auto iterator = enrollees.cbegin () + maxEnrollees;
             iterator != enrollees.cend (); iterator++)
        {
            output.push_back (*iterator);
            changed = true;
        }
    }
    return changed;
}

void Specialty::AddEnrolleeToOrder (Enrollee *enrollee, std::vector <Enrollee *> &queue)
{
    if (queue.size () == 0)
    {
        queue.push_back (enrollee);
        return;
    }
    else
    {
        for (auto iterator = queue.begin (); iterator != queue.end (); iterator++)
        {
            Enrollee *anotherEnrollee = *iterator;
            if (IsFirstEnrolleeBetter (this, enrollee, anotherEnrollee))
            {
                queue.insert (iterator, enrollee);
                return;
            }
        }

        queue.push_back (enrollee);
    }
}

bool Specialty::IsPedagogical () const
{
    return isPedagogical_;
}

void Specialty::SetIsPedagogical (bool isPedagogical)
{
    isPedagogical_ = isPedagogical;
}

void Specialty::AddAcceptedRODSubject (unsigned subject)
{
    if (!IsRODSubjectAccepted (subject))
    {
        acceptedRODSubjects_.push_back (subject);
    }
}

bool Specialty::RemoveAcceptedRODSubject (unsigned subject)
{
    auto iterator = std::find (acceptedRODSubjects_.begin (), acceptedRODSubjects_.end (), subject);
    if (iterator != acceptedRODSubjects_.end ())
    {
        acceptedRODSubjects_.erase (iterator);
        return true;
    }
    else
    {
        return false;
    }
}

bool Specialty::IsRODSubjectAccepted (unsigned subject) const
{
    auto iterator = std::find (acceptedRODSubjects_.cbegin (), acceptedRODSubjects_.cend (), subject);
    return iterator != acceptedRODSubjects_.cend ();
}

const std::vector <unsigned> &Specialty::GetAcceptedRODSubjects () const
{
    return acceptedRODSubjects_;
}

void Specialty::SaveToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, DeHashTable *deHashTable) const
{
    output->SetAttribute ("name", deHashTable->DeHash (id_).c_str ());
    output->SetAttribute ("maxEnrolleesInFreeForm", maxEnrolleesInFreeForm_);
    output->SetAttribute ("maxEnrolleesInPaidForm", maxEnrolleesInPaidForm_);
    output->SetAttribute ("isPedagogical", isPedagogical_);

    tinyxml2::XMLElement *requiredExamsElement = document.NewElement ("requiredExams");
    output->InsertEndChild (requiredExamsElement);

    for (auto iterator = requiredExams_.cbegin (); iterator != requiredExams_.cend (); iterator++)
    {
        tinyxml2::XMLElement *examElement = document.NewElement ("exam");
        requiredExamsElement->InsertEndChild (examElement);
        examElement->SetAttribute ("usedInPerExamComparision", iterator->first);

        const std::vector <unsigned> &subjects = iterator->second;
        for (auto subjectIterator = subjects.cbegin (); subjectIterator != subjects.cend (); subjectIterator++)
        {
            tinyxml2::XMLElement *subjectElement = document.NewElement ("subject");
            examElement->InsertEndChild (subjectElement);
            subjectElement->SetAttribute ("name", deHashTable->DeHash (*subjectIterator).c_str ());
        }
    }

    tinyxml2::XMLElement *marksInCertificatePriorityElement = document.NewElement ("marksInCertificatePriority");
    output->InsertEndChild (marksInCertificatePriorityElement);

    for (auto iterator = marksInCertificatePriority_.cbegin (); iterator != marksInCertificatePriority_.cend (); iterator++)
    {
        tinyxml2::XMLElement *subjectElement = document.NewElement ("subject");
        marksInCertificatePriorityElement->InsertEndChild (subjectElement);
        subjectElement->SetAttribute ("name", deHashTable->DeHash (*iterator).c_str ());
    }

    tinyxml2::XMLElement *acceptedRODSubjectsElement = document.NewElement ("acceptedRODSubjects");
    output->InsertEndChild (acceptedRODSubjectsElement);

    for (auto iterator = acceptedRODSubjects_.cbegin (); iterator != acceptedRODSubjects_.cend (); iterator++)
    {
        tinyxml2::XMLElement *subjectElement = document.NewElement ("subject");
        acceptedRODSubjectsElement->InsertEndChild (subjectElement);
        subjectElement->SetAttribute ("name", deHashTable->DeHash (*iterator).c_str ());
    }

    tinyxml2::XMLElement *enrolleesInFreeFormElement = document.NewElement ("enrolleesInFreeForm");
    output->InsertEndChild (enrolleesInFreeFormElement);

    for (auto iterator = enrolleesInFreeForm_.cbegin (); iterator != enrolleesInFreeForm_.cend (); iterator++)
    {
        Enrollee *enrollee = *iterator;
        tinyxml2::XMLElement *enrolleeElement = document.NewElement ("enrollee");

        enrolleesInFreeFormElement->InsertEndChild (enrolleeElement);
        enrolleeElement->SetAttribute ("passport", deHashTable->DeHash (enrollee->GetId ()).c_str ());
    }

    tinyxml2::XMLElement *enrolleesInPaidFormElement = document.NewElement ("enrolleesInPaidForm");
    output->InsertEndChild (enrolleesInPaidFormElement);

    for (auto iterator = enrolleesInPaidForm_.cbegin (); iterator != enrolleesInPaidForm_.cend (); iterator++)
    {
        Enrollee *enrollee = *iterator;
        tinyxml2::XMLElement *enrolleeElement = document.NewElement ("enrollee");

        enrolleesInPaidFormElement->InsertEndChild (enrolleeElement);
        enrolleeElement->SetAttribute ("passport", deHashTable->DeHash (enrollee->GetId ()).c_str ());
    }
}

void Specialty::LoadFromXML (tinyxml2::XMLElement *input, DeHashTable *deHashTable)
{

}
}
