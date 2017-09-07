#pragma once
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <list>

namespace UEADB
{
template <class T> class ReferenceCountedList : public UEAA::ReferenceCounted
{
public:
    ReferenceCountedList () : UEAA::ReferenceCounted (), data_ () {}
    virtual ~ReferenceCountedList () {}

    std::list <T> data_;
};
}
