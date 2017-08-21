#include <UEAA/BuildConfiguration.hpp>
#include "RepublicanOlympiadDiplomaType.hpp"

namespace UEAA
{
bool IsFirstDiplomaBetter (UEAA::RepublicanOlympiadDiplomaType first, UEAA::RepublicanOlympiadDiplomaType second)
{
    return first < second;
}
}
