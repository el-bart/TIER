/*
 * Point.hpp
 *
 * generic class representing n-Dimentional point in
 * Euklidean space (continues or discret).
 *
 * each variable MUST beahave as a type (value)!
 *
 */
#ifndef INCLUDE_VECTORS_POINT_HPP_FILE
#define INCLUDE_VECTORS_POINT_HPP_FILE

#include <cmath>
#include <assert.h>

#include "Vectors/Exception.hpp"


namespace Vectors
{

// Tdims    - dimmentions size
// TdimType - each dimmention type
template<unsigned int Tdims, typename TDimType>
class Point
{
public:
  Point(void)
  {
  };

  // return distance beetween two points
  inline double dist(const Vectors::Point<Tdims, TDimType> &p) const
  {
    double sum=0;
    for(unsigned int i=0; i<Tdims; ++i)
    {
      double di=_dim[i]-p._dim[i];
      sum+=di*di;
    };
    return ::sqrt(sum);
  };

  inline TDimType &operator[](unsigned int pos)
  {
    assert( pos<size() );
    return _dim[pos];
  };

  inline const TDimType &operator[](unsigned int pos) const
  {
    assert( pos<size() );
    return _dim[pos];
  };

  inline bool operator==(const Vectors::Point<Tdims,TDimType> &p) const
  {
    if( size()!=p.size() )      // size does not match?
      return false;

    for(unsigned int i=0; i<Tdims; ++i)
      if( (*this)[i]!=p[i] )
        return false;

    return true;
  };

  // returns dimmentions count
  inline unsigned int size(void) const
  {
    return Tdims;
  };

private:
  TDimType _dim[Tdims];
}; // class Point


//
// some commonly used dimmentions sizes
//

template <typename TDimType>
class Point2D: public Vectors::Point<2, TDimType>
{
public:
  Point2D(void)
  {
  };

  Point2D(const TDimType &x, const TDimType &y)
  {
    (*this)[0]=x;
    (*this)[1]=y;
  };
};

template <typename TDimType>
class Point3D: public Vectors::Point<3, TDimType>
{
public:
  Point3D(void)
  {
  };

  Point3D(const TDimType &x, const TDimType &y, const TDimType &z)
  {
    *this[0]=x;
    *this[1]=y;
    *this[2]=z;
  };
};


//
// some commolny used spaces type
//

typedef Point2D<double> Point2DCont;
typedef Point2D<int   > Point2DDisc;

typedef Point3D<double> Point3DCont;
typedef Point3D<int   > Point3DDisc;


}; // namespace Vectors


#endif

