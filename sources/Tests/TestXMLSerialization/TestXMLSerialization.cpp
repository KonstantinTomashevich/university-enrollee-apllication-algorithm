#include "TestXMLSerialization.hpp"
#include <iostream>
#include <ctime>
#include <Dependencies/TinyXML2/tinyxml2.h>

int main ()
{
    srand (time (0));
    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    InitHashes (deHashTable.GetTrackingObject ());

    UEAA::SharedPointer <UEAA::University> university (CreateTestUniversity ());
    if (university.GetTrackingObject () == nullptr)
    {
        std::cout << "Can't create university!" << std::endl;
        return 1;
    }

    for (unsigned index = 0; index < TECH_ENROLLEES; index++)
    {
        university->AddEnrollee (GenerateEnrollee (true, false, deHashTable));
    }

    for (unsigned index = 0; index < ARTS_ENROLLEES; index++)
    {
        university->AddEnrollee (GenerateEnrollee (false, true, deHashTable));
    }

    for (unsigned index = 0; index < TECH_AND_ARTS_ENROLLEES; index++)
    {
        university->AddEnrollee (GenerateEnrollee (true, true, deHashTable));
    }

    tinyxml2::XMLPrinter printer;
    {
        university->ProcessEnrolleesApplication ();
        tinyxml2::XMLDocument document;
        tinyxml2::XMLElement *universityElement = document.NewElement ("university");

        document.InsertEndChild (universityElement);
        university->SaveToXML (document, universityElement, deHashTable);
        document.Print (&printer);
    }

    UEAA::SharedPointer <UEAA::University> anotherUniversity (new UEAA::University ());
    {
        tinyxml2::XMLDocument document;
        document.Parse (printer.CStr (), printer.CStrSize ());
        anotherUniversity->LoadFromXML (document.FirstChildElement ("university"), deHashTable);
    }

    bool isUniversitiesEquals = *university.GetTrackingObject () == *anotherUniversity.GetTrackingObject ();
    std::cout << "Is universities equals: " << isUniversitiesEquals << std::endl;
    return isUniversitiesEquals ? 0 : 1;
}

void InitHashes (UEAA::DeHashTable *deHashTable)
{
    MATH_EXAM = UEAA::CStringToHash ("Math", deHashTable);
    PHYSICS_EXAM = UEAA::CStringToHash ("Physics", deHashTable);
    LANGUAGE_EXAM = UEAA::CStringToHash ("Language", deHashTable);
    SOCIETY_EXAM = UEAA::CStringToHash ("Society", deHashTable);
    ARTS_INTERNAL_EXAM = UEAA::CStringToHash ("Arts Internal", deHashTable);

    TECH_FACULTY = UEAA::CStringToHash ("Tech Faculty", deHashTable);
    TechFaculty::APPLIED_COMPUTER_SCIENCE_FREE = UEAA::CStringToHash ("Applied Computer Science (Free)", deHashTable);
    TechFaculty::COMPUTER_SCIENCE_FREE = UEAA::CStringToHash ("Computer Science (Free)", deHashTable);

    TechFaculty::APPLIED_COMPUTER_SCIENCE_PAID = UEAA::CStringToHash ("Applied Computer Science (Paid)", deHashTable);
    TechFaculty::COMPUTER_SCIENCE_PAID = UEAA::CStringToHash ("Computer Science (Paid)", deHashTable);

    ARTS_FACULTY = UEAA::CStringToHash ("Arts Faculty", deHashTable);
    ArtsFaculty::PAINTING_FREE = UEAA::CStringToHash ("Painting (Free)", deHashTable);
    ArtsFaculty::THEATRE_FREE = UEAA::CStringToHash ("Theatre (Free)", deHashTable);

    ArtsFaculty::PAINTING_PAID = UEAA::CStringToHash ("Painting (Paid)", deHashTable);
    ArtsFaculty::THEATRE_PAID = UEAA::CStringToHash ("Theatre (Paid)", deHashTable);
}

UEAA::Enrollee *GenerateEnrollee (bool addTech, bool addArts, UEAA::DeHashTable *deHashTable)
{
    std::string passportNumber = std::to_string (enrolleesGeneratorCounter_);
    while (passportNumber.size () < 7)
    {
        passportNumber = "0" + passportNumber;
    }
    UEAA::Enrollee *enrollee = new UEAA::Enrollee (UEAA::CStringToHash (("XX" + passportNumber).c_str (), deHashTable));

    enrollee->SetCertificateMark (MATH_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (PHYSICS_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (LANGUAGE_EXAM, 5 + rand () % 6);
    enrollee->SetCertificateMark (SOCIETY_EXAM, 5 + rand () % 6);
    enrollee->CalculateCertificateMedianMark ();
    enrollee->CheckIsHasSchoolGoldMedal ();

    enrollee->SetExamResult (LANGUAGE_EXAM, 30 + rand () % 71);
    if (addTech)
    {
        enrollee->SetExamResult (MATH_EXAM, 30 + rand () % 71);
        enrollee->SetExamResult (PHYSICS_EXAM, 30 + rand () % 71);
    }

    if (addArts)
    {
        enrollee->SetExamResult (SOCIETY_EXAM, 30 + rand () % 71);
        enrollee->SetExamResult (ARTS_INTERNAL_EXAM, 30 + rand () % 71);
    }

    if (addTech)
    {
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (TECH_FACULTY, TechFaculty::APPLIED_COMPUTER_SCIENCE_FREE));
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (TECH_FACULTY, TechFaculty::COMPUTER_SCIENCE_FREE));

        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (TECH_FACULTY, TechFaculty::APPLIED_COMPUTER_SCIENCE_PAID));
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (TECH_FACULTY, TechFaculty::COMPUTER_SCIENCE_PAID));
    }

    if (addArts)
    {
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (ARTS_FACULTY, ArtsFaculty::PAINTING_FREE));
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (ARTS_FACULTY, ArtsFaculty::THEATRE_FREE));

        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (ARTS_FACULTY, ArtsFaculty::PAINTING_PAID));
        enrollee->AddChoiceToBack (new UEAA::EnrolleeChoice (ARTS_FACULTY, ArtsFaculty::THEATRE_PAID));
    }
    enrolleesGeneratorCounter_++;
    return enrollee;
}

UEAA::University *CreateTestUniversity ()
{
    UEAA::University *university = new UEAA::University ();
    UEAA::SharedPointer <UEAA::Faculty> techFaculty (CreateTechFaculty (university));
    if (!university->AddFaculty (techFaculty))
    {
        std::cout << "Can't add tech faculty!" << std::endl;
        delete university;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Faculty> artsFaculty (CreateArtsFaculty (university));
    if (!university->AddFaculty (artsFaculty))
    {
        std::cout << "Can't add arts faculty!" << std::endl;
        delete university;
        return nullptr;
    }
    return university;
}

UEAA::Faculty *CreateTechFaculty (UEAA::University *university)
{
    UEAA::Faculty *techFaculty = new UEAA::Faculty (university, TECH_FACULTY);
    UEAA::SharedPointer <UEAA::Specialty> acsFreeSpecialty (
            CreateTechSpecialty (techFaculty, TechFaculty::APPLIED_COMPUTER_SCIENCE_FREE, TechFaculty::ACS_MAX_ENROLLEES_IN_FREE));
    if (!techFaculty->AddSpecialty (acsFreeSpecialty))
    {
        std::cout << "Can't add ACS free specialty!" << std::endl;
        delete techFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> acsPaidSpecialty (
            CreateTechSpecialty (techFaculty, TechFaculty::APPLIED_COMPUTER_SCIENCE_PAID, TechFaculty::ACS_MAX_ENROLLEES_IN_PAID));
    if (!techFaculty->AddSpecialty (acsPaidSpecialty))
    {
        std::cout << "Can't add ACS paid specialty!" << std::endl;
        delete techFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> csFreeSpecialty (
            CreateTechSpecialty (techFaculty, TechFaculty::COMPUTER_SCIENCE_FREE, TechFaculty::CS_MAX_ENROLLEES_IN_FREE));
    if (!techFaculty->AddSpecialty (csFreeSpecialty))
    {
        std::cout << "Can't add CS free specialty!" << std::endl;
        delete techFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> csPaidSpecialty (
            CreateTechSpecialty (techFaculty, TechFaculty::COMPUTER_SCIENCE_PAID, TechFaculty::CS_MAX_ENROLLEES_IN_PAID));
    if (!techFaculty->AddSpecialty (csPaidSpecialty))
    {
        std::cout << "Can't add CS paid specialty!" << std::endl;
        delete techFaculty;
        return nullptr;
    }
    return techFaculty;
}

UEAA::Specialty *CreateTechSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees)
{
    UEAA::Specialty *specialty = new UEAA::Specialty (faculty, id);
    specialty->SetMaxEnrollees (maxEnrollees);
    specialty->AddAcceptedRODSubject (MATH_EXAM);

    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {MATH_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {PHYSICS_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (false, {LANGUAGE_EXAM}));
        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (MATH_EXAM);
        certPriority.push_back (PHYSICS_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
    return specialty;
}

UEAA::Faculty *CreateArtsFaculty (UEAA::University *university)
{
    UEAA::Faculty *artsFaculty = new UEAA::Faculty (university, ARTS_FACULTY);
    UEAA::SharedPointer <UEAA::Specialty> paintingFreeSpecialty (
            CreateArtsSpecialty (artsFaculty, ArtsFaculty::PAINTING_FREE, ArtsFaculty::PAINTING_MAX_ENROLLEES_IN_FREE));

    if (!artsFaculty->AddSpecialty (paintingFreeSpecialty))
    {
        std::cout << "Can't add Painting free specialty!" << std::endl;
        delete artsFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> paintingPaidSpecialty (
            CreateArtsSpecialty (artsFaculty, ArtsFaculty::PAINTING_PAID, ArtsFaculty::PAINTING_MAX_ENROLLEES_IN_PAID));

    if (!artsFaculty->AddSpecialty (paintingPaidSpecialty))
    {
        std::cout << "Can't add Painting paid specialty!" << std::endl;
        delete artsFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> theatreFreeSpecialty (
            CreateArtsSpecialty (artsFaculty, ArtsFaculty::THEATRE_FREE, ArtsFaculty::THEATRE_MAX_ENROLLEES_IN_FREE));

    if (!artsFaculty->AddSpecialty (theatreFreeSpecialty))
    {
        std::cout << "Can't add Theatre free specialty!" << std::endl;
        delete artsFaculty;
        return nullptr;
    }

    UEAA::SharedPointer <UEAA::Specialty> theatrePaidSpecialty (
            CreateArtsSpecialty (artsFaculty, ArtsFaculty::THEATRE_PAID, ArtsFaculty::THEATRE_MAX_ENROLLEES_IN_PAID));

    if (!artsFaculty->AddSpecialty (theatrePaidSpecialty))
    {
        std::cout << "Can't add Theatre paid specialty!" << std::endl;
        delete artsFaculty;
        return nullptr;
    }
    return artsFaculty;
}

UEAA::Specialty *CreateArtsSpecialty (UEAA::Faculty *faculty, unsigned id, unsigned maxEnrollees)
{
    UEAA::Specialty *specialty = new UEAA::Specialty (faculty, id);
    specialty->SetMaxEnrollees (maxEnrollees);
    specialty->AddAcceptedRODSubject (SOCIETY_EXAM);
    specialty->SetIsPedagogical (true);

    {
        std::vector <std::pair <bool, std::vector <unsigned> > > exams;
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {ARTS_INTERNAL_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (true, {SOCIETY_EXAM}));
        exams.push_back (std::pair <bool, std::vector <unsigned> > (false, {LANGUAGE_EXAM}));
        specialty->SetRequiredExams (exams);
    }

    {
        std::vector <unsigned> certPriority;
        certPriority.push_back (SOCIETY_EXAM);
        specialty->SetMarksInCertificatePriority (certPriority);
    }
    return specialty;
}

void PrintEnrollee (const UEAA::Enrollee *enrollee, const UEAA::DeHashTable *deHashTable)
{
    std::cout << "Enrollee " << deHashTable->DeHash (enrollee->GetId ()) << ":" << std::endl <<
              "    Median:   " << enrollee->GetCertificateMedianMark () * 1 << std::endl <<
              "    Math:     " << enrollee->GetExamResult (MATH_EXAM) * 1 << std::endl <<
              "    Physics:  " << enrollee->GetExamResult (PHYSICS_EXAM) * 1 << std::endl <<
              "    Language: " << enrollee->GetExamResult (LANGUAGE_EXAM) * 1 << std::endl <<
              "    Society:  " << enrollee->GetExamResult (SOCIETY_EXAM) * 1 << std::endl <<
              "    Arts:     " << enrollee->GetExamResult (ARTS_INTERNAL_EXAM) * 1 << std::endl <<
              std::endl;
}
