#ifndef BOOST_EXTENT_RANGE_RG071801_HPP
#define BOOST_EXTENT_RANGE_RG071801_HPP

#include <utility>

namespace boost {
namespace detail {
namespace multi_array {

template <typename Extent, typename SizeType>
class extent_range : private std::pair<Extent,Extent> {
  typedef std::pair<Extent,Extent> super_type;
public:
  typedef Extent index;
  typedef SizeType size_type;

  extent_range(index start, index finish) :
    super_type(start,finish) { }

  extent_range(index finish) :
    super_type(0,finish) { }

  extent_range() : super_type(0,0) { }

  index start() const { return super_type::first; }

  index finish() const { return super_type::second; }

  size_type size() const { return super_type::second - super_type::first; }
};

} // namespace multi_array
} // namespace detail 
} // namespace boost


#endif // BOOST_EXTENT_RANGE_RG071801_HPP
