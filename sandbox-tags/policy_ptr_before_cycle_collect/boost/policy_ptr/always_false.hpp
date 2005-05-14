#ifndef ALWAYS_FALSE_HPP
#define ALWAYS_FALSE_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <typename T>
    struct always_false
    {
        enum { value = 0 };
    };
    //----------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // ALWAYS_FALSE_HPP
