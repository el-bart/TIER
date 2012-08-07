/*
 * Base.hpp
 *
 * representation of base for a given space.
 *
 */
#ifndef INCLUDE_VECTORS_BASE_HPP_FILE
#define INCLUDE_VECTORS_BASE_HPP_FILE

#include <memory>

#include "Vectors/Vector.hpp"
#include "Vectors/Point.hpp"


namespace Vectors
{

template <unsigned int Tdims, typename TDimType>
class Base
{
public:
  typename Vectors::Point<Tdims, TDimType> Point_t;
  typename Vectors::Vector<Point_t>        Vector_t;

  Base(void):
    _base(Tdims)
  {
  };

  inline TDimType &operator[](unsigned int pos)
  {
    assert( pos<dims() );
    return _base[pos];
  };
  inline const TDimType &operator[](unsigned int pos) const
  {
    assert( pos<dims() );
    return _base[pos];
  };

  inline unsigned int dims(void) const
  {
    return Tdims;
  };


private:
  std::vector<Vector_t> _base;
};


//
// some commonly used sizes
//

template<typename TDimType>
class Base2D: public Vectors::Base<2, TDimType>
{
public:
  Base2D(void)
  {
  };
  Base2D(const Vector_t &v1, const Vector_t &v2)
  {
    *this[0]=v1;
    *this[1]=v2;
  };
};

template<typename TDimType>
class Base3D: public Vectors::Base<3, TDimType>
{
public:
  Base3D(void)
  {
  };
  Base3D(const Vector_t &v1, const Vector_t &v2, const Vector_t &v3)
  {
    *this[0]=v1;
    *this[1]=v2;
    *this[2]=v3;
  };
};


//
// some commonly used space types
//

typedef Base2D<double> Base2DCont;
typedef Base2D<int   > Base2DDisc;

typedef Base3D<double> Base3DCont;
typedef Base3D<int   > Base3DDisc;


}; // namespace Vectors

#endif

