#include "EnrolleeRandomizer.hpp"
#include <UEAA/Utils/DeHashTable.hpp>
#include <UEAA/Utils/SharedPointer.hpp>

#include <UEAA/Utils/CStringToHash.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>
#include <iostream>

namespace EnrolleeRandomizer
{
unsigned Execute (const wchar_t *arguments)
{
    return Execute (ParseArgumentsString (arguments));
}

unsigned Execute (int argumentsCount, char **arguments)
{
    std::vector <std::string> argumentsVector;
    // Because first arguments is always executable path.
    if (argumentsCount > 1)
    {
        for (unsigned index = 1; index < argumentsCount; index++)
        {
            argumentsVector.emplace_back (std::string (arguments[index]));
        }
    }
    return Execute (argumentsVector);
}

unsigned Execute (const std::vector <std::string> &arguments)
{
    if (arguments.size () != 5)
    {
        PrintHelp ();
        return 0;
    }

    std::string templateXMLPath = arguments.at (0);
    std::string outputDirectory = arguments.at (1);
    unsigned toGenerateCount = 0;
    unsigned examsDelta = 0;
    unsigned marksDelta = 0;

    if (!tinyxml2::XMLUtil::ToUnsigned (arguments.at (2).c_str (), &toGenerateCount) ||
        !tinyxml2::XMLUtil::ToUnsigned (arguments.at (3).c_str (), &examsDelta) ||
        !tinyxml2::XMLUtil::ToUnsigned (arguments.at (4).c_str (), &marksDelta))
    {
        std::cout << "Error, can not parse arguments!" << std::endl;
        return 1;
    }

    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    std::string templateEnrolleePassport;
    tinyxml2::XMLDocument document;

    if (document.LoadFile (templateXMLPath.c_str ()) != tinyxml2::XML_SUCCESS)
    {
        std::cout << "Error, can not load xml template!" << std::endl;
        return 1;
    }

    tinyxml2::XMLElement *rootElement = document.FirstChildElement ("enrollee");
    if (rootElement == nullptr)
    {
        std::cout << "Error, can not find <enrollee> root element!" << std::endl;
        return 1;
    }
    templateEnrolleePassport = rootElement->Attribute ("passport");

    unsigned enrolleePassportNumber = 0;
    if (!tinyxml2::XMLUtil::ToUnsigned (templateEnrolleePassport.substr (2, 7).c_str (), &enrolleePassportNumber))
    {
        enrolleePassportNumber = 0;
    }

    for (unsigned index = 0; index < toGenerateCount; index++, enrolleePassportNumber++)
    {
        std::cout << (index + 1) << "/" << toGenerateCount << ": generating GE" << enrolleePassportNumber << "." << std::endl;
        GenerateNextEnrollee (rootElement, outputDirectory, enrolleePassportNumber, examsDelta, marksDelta, deHashTable);
    }
    return 0;
}

std::vector <std::string> ParseArgumentsString (const wchar_t *arguments)
{
    size_t length = wcslen (arguments);
    char *cArguments = new char[length];
    wcstombs (cArguments, arguments, length);

    std::vector <std::string> argumentsVector;
    std::string read = "";
    bool isQuoteOpened = false;
    bool executablePathSkipped = false;

    for (unsigned index = 0; index < length; index++)
    {
        char symbol = cArguments[index];
        if (symbol == '\"')
        {
            isQuoteOpened = !isQuoteOpened;
        }

        else if (isspace (symbol))
        {
            if (isQuoteOpened)
            {
                read += " ";
            }
            else if (executablePathSkipped)
            {
                if (!read.empty ())
                {
                    argumentsVector.push_back (read);
                }
                read = "";
            }
            else
            {
                executablePathSkipped = true;
                read = "";
            }
        }

        else
        {
            read += symbol;
        }
    }

    delete [] cArguments;
    if (executablePathSkipped)
    {
        argumentsVector.push_back (read);
    }
    return argumentsVector;
}

void PrintHelp ()
{
    std::cout << "Enrollee Randomizer expected arguments:" << std::endl <<
                 "    1. Path to template enrollee XML file." << std::endl <<
                 "    2. Folder in which generated enrollees will be saved." << std::endl <<
                 "    3. Enrolees count." << std::endl <<
                 "    4. Exams randomization delta (new result = template result + random (-delta, +delta)." << std::endl <<
                 "    5. Marks randomization delta (new mark = template mark + random (-delta, +delta)." << std::endl;
}

void GenerateNextEnrollee (tinyxml2::XMLElement *enrolleeElement, const std::string &outputDirectory,
                           unsigned passportNumber, unsigned examsDelta, unsigned marksDelta,
                           UEAA::DeHashTable *deHashTable)
{
    std::string passportNumberString = std::to_string (passportNumber);
    while (passportNumberString.size () < 7)
    {
        passportNumberString = "0" + passportNumberString;
    }

    UEAA::SharedPointer <UEAA::Enrollee> enrollee (new UEAA::Enrollee (
             UEAA::CalculateEnrolleeHash ("GE", passportNumberString.c_str (), deHashTable)));
    enrollee->LoadFromXML (enrolleeElement, deHashTable);

    std::vector <unsigned> examsSubjects = enrollee->GetPassedExamsSubjects ();
    for (auto iterator = examsSubjects.begin (); iterator != examsSubjects.end (); iterator++)
    {
        enrollee->SetExamResult (*iterator, enrollee->GetExamResult (*iterator) + (rand () % (examsDelta * 2) - examsDelta));
    }

    std::vector <unsigned> marksSubjects = enrollee->GetCertificateMarksSubjects ();
    for (auto iterator = marksSubjects.begin (); iterator != marksSubjects.end (); iterator++)
    {
        enrollee->SetCertificateMark (*iterator, enrollee->GetExamResult (*iterator) + (rand () % (marksDelta * 2) - marksDelta));
    }

    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement *saveElement = document.NewElement ("enrollee");
    document.InsertFirstChild (saveElement);
    enrollee->SaveToXML (document, saveElement, deHashTable);
    document.SaveFile ((outputDirectory + "/GE" + passportNumberString + ".xml").c_str ());
}
}
