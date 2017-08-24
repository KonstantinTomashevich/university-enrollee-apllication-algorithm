#pragma once
#include <UEAA/Core/Enrollee/StudyForm.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <Dependencies/TinyXML2/tinyxml2.h>

namespace UEAA
{
struct EnrolleeChoice
{
public:
    unsigned faculty_;
    unsigned specialty_;
    StudyForm studyForm_;

    EnrolleeChoice ();
    EnrolleeChoice (unsigned faculty, unsigned specialty, StudyForm studyForm);

    void SaveToXML (tinyxml2::XMLElement *output, DeHashTable *deHashTable) const;
};
const EnrolleeChoice EMPTY_ENROLLEE_CHOICE;
}
