#pragma once
namespace UEAA
{
enum RODType
{
    ROD_FIRST = 1,
    ROD_SECOND = 2,
    ROD_THIRD = 3,
    ROD_NONE = 4
};

bool IsFirstDiplomaBetter (RODType first, RODType second);
}

