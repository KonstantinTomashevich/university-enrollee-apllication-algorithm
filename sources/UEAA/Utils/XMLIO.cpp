#include <UEAA/BuildConfiguration.hpp>
#include "XMLIO.hpp"

#include <Dependencies/TinyXML2/tinyxml2.h>
#include <iostream>

namespace UEAA
{
namespace XMLIO
{
University *LoadFromFile (const char *fileName, DeHashTable *deHashTable)
{
    return nullptr;
}

bool SaveToFile (const char *fileName, University *university, DeHashTable *deHashTable)
{
    tinyxml2::XMLDocument document;
    SaveUniversityToXML (document, university, deHashTable);

    if (document.SaveFile (fileName, false) == tinyxml2::XML_SUCCESS)
    {
        return true;
    }
    else
    {
        std::cout << "Couldn't save university to file " << fileName << "!" << std::endl;
        return false;
    }
}

void SaveUniversityToXML (tinyxml2::XMLDocument &document, University *university, DeHashTable *deHashTable)
{
    tinyxml2::XMLElement *root = document.NewElement ("university");
    document.InsertFirstChild (root);

    tinyxml2::XMLElement *facultiesElement = document.NewElement ("faculties");
    root->InsertEndChild (facultiesElement);

    for (int index = 0; index < university->GetFacultiesCount (); index++)
    {
        Faculty *faculty = university->GetFacultyByIndex (index);
        tinyxml2::XMLElement *facultyElement = document.NewElement ("faculty");

        facultiesElement->InsertEndChild (facultyElement);
        SaveFacultyToXML (document, facultyElement, faculty, deHashTable);
    }

    tinyxml2::XMLElement *enrolleesElement = document.NewElement ("enrollees");
    root->InsertEndChild (enrolleesElement);

    for (int index = 0; index < university->GetEnrolleesCount (); index++)
    {
        Enrollee *enrollee = university->GetEnrolleeByIndex (index);
        tinyxml2::XMLElement *enrolleeElement = document.NewElement ("enrollee");
        enrolleesElement->InsertEndChild (enrolleeElement);
        SaveEnrolleeToXML (document, enrolleeElement, enrollee, deHashTable);
    }
}

void SaveFacultyToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Faculty *faculty, DeHashTable *deHashTable)
{
    output->SetAttribute ("name", deHashTable->DeHash (faculty->GetId ()).c_str ());
    for (int index = 0; index < faculty->GetSpecialtiesCount (); index++)
    {
        Specialty *specialty = faculty->GetSpecialtyByIndex (index);
        tinyxml2::XMLElement *specialtyElement = document.NewElement ("specialty");

        output->InsertEndChild (specialtyElement);
        SaveSpecialtyToXML (document, specialtyElement, specialty, deHashTable);
    }
}

void SaveSpecialtyToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Specialty *specialty, DeHashTable *deHashTable)
{
    output->SetAttribute ("name", deHashTable->DeHash (specialty->GetId ()).c_str ());
    output->SetAttribute ("maxEnrolleesInFreeForm", specialty->GetMaxEnrolleesInFreeForm ());
    output->SetAttribute ("maxEnrolleesInPaidForm", specialty->GetMaxEnrolleesInPaidForm ());
    output->SetAttribute ("isPedagogical", specialty->IsPedagogical ());

    const std::vector <std::pair <bool, std::vector <unsigned> > > &requiredExams = specialty->GetRequiredExams ();
    tinyxml2::XMLElement *requiredExamsElement = document.NewElement ("requiredExams");
    output->InsertEndChild (requiredExamsElement);

    for (auto iterator = requiredExams.cbegin (); iterator != requiredExams.cend (); iterator++)
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

    const std::vector <unsigned> &marksInCertificatePriority = specialty->GetMarksInCertificatePriority ();
    tinyxml2::XMLElement *marksInCertificatePriorityElement = document.NewElement ("marksInCertificatePriority");
    output->InsertEndChild (marksInCertificatePriorityElement);

    for (auto iterator = marksInCertificatePriority.cbegin ();
            iterator != marksInCertificatePriority.cend (); iterator++)
    {
        tinyxml2::XMLElement *subjectElement = document.NewElement ("subject");
        marksInCertificatePriorityElement->InsertEndChild (subjectElement);
        subjectElement->SetAttribute ("name", deHashTable->DeHash (*iterator).c_str ());
    }

    const std::vector <unsigned> &acceptedRODSubjects = specialty->GetAcceptedRODSubjects ();
    tinyxml2::XMLElement *acceptedRODSubjectsElement = document.NewElement ("acceptedRODSubjects");
    output->InsertEndChild (acceptedRODSubjectsElement);

    for (auto iterator = acceptedRODSubjects.cbegin ();
         iterator != acceptedRODSubjects.cend (); iterator++)
    {
        tinyxml2::XMLElement *subjectElement = document.NewElement ("subject");
        acceptedRODSubjectsElement->InsertEndChild (subjectElement);
        subjectElement->SetAttribute ("name", deHashTable->DeHash (*iterator).c_str ());
    }
}

void SaveEnrolleeToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Enrollee *enrollee, DeHashTable *deHashTable)
{

}
}
}
