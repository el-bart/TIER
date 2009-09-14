/*
 * Space.hpp
 *
 * template class representing space with some lines
 * int it, represented by abstract Line template class.
 *
 */
#ifndef INCLUDE_VECTORS_SPACE_HPP_FILE
#define INCLUDE_VECTORS_SPACE_HPP_FILE

#include <list>

#include "Vectors/Line.hpp"
#include "Vectors/Exception.hpp"



namespace Vectors
{

//
// space representation itself
//
template <typename TLine>
class Space
{
public:
  // some useful typedefs
  typedef typename std::list<TLine>::iterator       iterator;
  typedef typename std::list<TLine>::const_iterator const_iterator;
  typedef std::auto_ptr< Vectors::Space<TLine> >    AutoSpace;

  // collection I/O
  inline void add(const TLine &l)
  {
    _elems.insert( _elems.begin(), l);  // add at the begining
  };
  inline void remove(iterator &it)
  {
    _elems.erase(it);
  };

  // note: this call is O(n) !!
  inline unsigned int linesCnt(void) const
  {
    return _elems.size();
  };

  // get iterators
  inline iterator begin(void)
  {
    return iterator( _elems.begin() );
  };
  inline iterator end(void)
  {
    return iterator( _elems.end()   );
  };

  // get const_iterators
  inline const_iterator begin(void) const
  {
    return const_iterator( _elems.begin() );
  };
  inline const_iterator end(void)   const
  {
    return const_iterator( _elems.end()   );
  };

protected:
  std::list<TLine> _elems;
}; // class Space


}; // namespace Vectors

#endif

