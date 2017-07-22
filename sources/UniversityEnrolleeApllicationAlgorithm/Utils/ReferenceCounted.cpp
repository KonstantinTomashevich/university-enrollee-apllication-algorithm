#include "ReferenceCounted.hpp"

namespace UniversityEnrolleeApllicationAlgorithm
{
ReferenceCounted::ReferenceCounted () :
    references_ (0)
{

}

ReferenceCounted::~ReferenceCounted ()
{

}

unsigned ReferenceCounted::GetReferencesCount () const
{
    return references_;
}

void ReferenceCounted::AddReference ()
{
    references_++;
}

void ReferenceCounted::SubtractReference ()
{
    if (references_ > 0)
    {
        references_--;
    }
}
}
