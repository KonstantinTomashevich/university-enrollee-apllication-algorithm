#include <UEAA/BuildConfiguration.hpp>
#include "Specialty.hpp"
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <algorithm>

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

const std::list <Enrollee *> & Specialty::GetEnrolleesInFreeForm () const
{
    return enrolleesInFreeForm_;
}

const std::list <Enrollee *> & Specialty::GetEnrolleesInPaidForm () const
{
    return enrolleesInPaidForm_;
}

bool Specialty::AddEnrollee (Enrollee *enrollee)
{
    EnrolleeChoice *choice = enrollee->GetCurrentChoice ();
    if (choice->GetSpecialty () != id_)
    {
        return false;
    }

    if (!CanEnrolleeChoiceSpecialty (this, enrollee))
    {
        return false;
    }

    AddEnrolleeToOrder (enrollee, (choice->GetStudyForm () == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_);
    return true;
}

std::list <Enrollee *> Specialty::RemoveExcessEnrollees ()
{
    std::list <Enrollee *> excessEnrollees;
    RemoveExcessEnrolleesOfStudyForm (excessEnrollees, STUDY_FORM_FREE);
    RemoveExcessEnrolleesOfStudyForm (excessEnrollees, STUDY_FORM_PAID);
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

void Specialty::RemoveExcessEnrolleesOfStudyForm (std::list <Enrollee *> &output, StudyForm studyForm)
{
    std::list <Enrollee *> &enrollees = (studyForm == STUDY_FORM_FREE) ? enrolleesInFreeForm_ : enrolleesInPaidForm_;
    unsigned maxEnrollees = (studyForm == STUDY_FORM_FREE) ? maxEnrolleesInFreeForm_ : maxEnrolleesInPaidForm_;

    while (enrollees.size () > maxEnrollees)
    {
        auto lastIterator = enrollees.cend ();
        lastIterator--;
        output.emplace_back (*lastIterator);
        enrollees.pop_back ();
    }
}

void Specialty::AddEnrolleeToOrder (Enrollee *enrollee, std::list <Enrollee *> &queue)
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
    Clear ();
    tinyxml2::XMLUtil::ToUnsigned (input->Attribute ("maxEnrolleesInFreeForm"), &maxEnrolleesInFreeForm_);
    tinyxml2::XMLUtil::ToUnsigned (input->Attribute ("maxEnrolleesInPaidForm"), &maxEnrolleesInPaidForm_);
    tinyxml2::XMLUtil::ToBool (input->Attribute ("isPedagogical"), &isPedagogical_);

    tinyxml2::XMLElement *requiredExamsElement = input->FirstChildElement ("requiredExams");
    if (requiredExamsElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = requiredExamsElement->FirstChildElement ("exam");
                element != nullptr; element = element->NextSiblingElement ("exam"))
        {
            bool usedInPerExamComparision = false;
            tinyxml2::XMLUtil::ToBool (element->Attribute ("usedInPerExamComparision"), &usedInPerExamComparision);
            std::vector <unsigned> subjects;

            for (tinyxml2::XMLElement *subjectElement = element->FirstChildElement ("subject");
                 subjectElement != nullptr; subjectElement = subjectElement->NextSiblingElement ("subject"))
            {
                subjects.push_back (CStringToHash (subjectElement->Attribute ("name"), deHashTable));
            }
            
            requiredExams_.push_back (std::pair <bool, std::vector <unsigned> > (usedInPerExamComparision, subjects));
        }
    }

    tinyxml2::XMLElement *marksInCertificatePriorityElement = input->FirstChildElement ("marksInCertificatePriority");
    if (marksInCertificatePriorityElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = marksInCertificatePriorityElement->FirstChildElement ("subject");
             element != nullptr; element = element->NextSiblingElement ("subject"))
        {
            marksInCertificatePriority_.push_back (CStringToHash (element->Attribute ("name"), deHashTable));
        }
    }

    tinyxml2::XMLElement *acceptedRODSubjectsElement = input->FirstChildElement ("acceptedRODSubjects");
    if (acceptedRODSubjectsElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = acceptedRODSubjectsElement->FirstChildElement ("subject");
             element != nullptr; element = element->NextSiblingElement ("subject"))
        {
            acceptedRODSubjects_.push_back (CStringToHash (element->Attribute ("name"), deHashTable));
        }
    }

    tinyxml2::XMLElement *enrolleesInFreeFormElement = input->FirstChildElement ("enrolleesInFreeForm");
    if (enrolleesInFreeFormElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = enrolleesInFreeFormElement->FirstChildElement ("enrollee");
             element != nullptr; element = element->NextSiblingElement ("enrollee"))
        {
            std::string passport = element->Attribute ("passport");
            enrolleesInFreeForm_.push_back (parent_->GetParent ()->GetEnrollee (passport.substr (0, 2), passport.substr (2, 7)));
        }
    }

    tinyxml2::XMLElement *enrolleesInPaidFormElement = input->FirstChildElement ("enrolleesInPaidForm");
    if (enrolleesInPaidFormElement != nullptr)
    {
        for (tinyxml2::XMLElement *element = enrolleesInPaidFormElement->FirstChildElement ("enrollee");
             element != nullptr; element = element->NextSiblingElement ("enrollee"))
        {
            std::string passport = element->Attribute ("passport");
            enrolleesInPaidForm_.push_back (parent_->GetParent ()->GetEnrollee (passport.substr (0, 2), passport.substr (2, 7)));
        }
    }
}

void Specialty::Clear ()
{
    requiredExams_.clear ();
    marksInCertificatePriority_.clear ();
    enrolleesInFreeForm_.clear ();
    enrolleesInPaidForm_.clear ();

    maxEnrolleesInFreeForm_ = 0;
    maxEnrolleesInPaidForm_ = 0;
    isPedagogical_ = false;
    acceptedRODSubjects_.clear ();
}

bool Specialty::operator == (const Specialty &rhs) const
{
    if (id_ != rhs.id_ ||
        requiredExams_ != rhs.requiredExams_ ||
        marksInCertificatePriority_ != rhs.marksInCertificatePriority_ ||
        maxEnrolleesInFreeForm_ != rhs.maxEnrolleesInFreeForm_ ||
        maxEnrolleesInPaidForm_ != rhs.maxEnrolleesInPaidForm_ ||
        isPedagogical_ != rhs.isPedagogical_ ||
        acceptedRODSubjects_ != rhs.acceptedRODSubjects_)
    {
        return false;
    }

    if (enrolleesInFreeForm_.size () == rhs.enrolleesInFreeForm_.size ())
    {
        auto firstIterator = enrolleesInFreeForm_.cbegin ();
        auto secondIterator = rhs.enrolleesInFreeForm_.cbegin ();

        for (; firstIterator != enrolleesInFreeForm_.cend () && secondIterator != rhs.enrolleesInFreeForm_.cend ();
               firstIterator++, secondIterator++)
        {
            if (**firstIterator != **secondIterator)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    if (enrolleesInPaidForm_.size () == rhs.enrolleesInPaidForm_.size ())
    {
        auto firstIterator = enrolleesInPaidForm_.cbegin ();
        auto secondIterator = rhs.enrolleesInPaidForm_.cbegin ();

        for (; firstIterator != enrolleesInPaidForm_.cend () && secondIterator != rhs.enrolleesInPaidForm_.cend ();
               firstIterator++, secondIterator++)
        {
            if (**firstIterator != **secondIterator)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool Specialty::operator != (const Specialty &rhs) const
{
    return !(rhs == *this);
}
}
