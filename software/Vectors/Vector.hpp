/*
 * Vector.hpp
 *
 * template class representing general vector in some
 * space (determined by point type class).
 *
 */
#ifndef INCLUDE_VECTORS_VECTOR_HPP_FILE
#define INCLUDE_VECTORS_VECTOR_HPP_FILE

#include <math.h>

#include "Vectors/Point.hpp"
#include "Vectors/Exception.hpp"


namespace Vectors
{

// we implement Vector as Point in equivalen space
template <typename TPoint>
class Vector: public TPoint
{
public:
  Vector(void)
  {
  };

  // make vector p1 -> p2
  Vector(const TPoint &p1, const TPoint &p2)
  {
    assert( p1.size()==p2.size()      );
    assert( p1.size()==TPoint::size() );
    unsigned int dims=p1.size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]=p2[i]-p1[i];
  };

  // distance from [0...0] -> *this
  inline double length(void) const
  {
    TPoint       pnt;
    unsigned int dims=pnt.size();
    for(unsigned int i=0; i<dims; ++i)
      pnt[i]=0;

    return dist(pnt);
  };

  // adds two Vectors
  inline Vectors::Vector<TPoint> &operator+=(const Vectors::Vector<TPoint> &v)
  {
    assert( TPoint::size()==v.size() );
    unsigned int dims=TPoint::size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]+=v[i];

    return *this;
  };

  // multiply Vector by some value (enlarge vector)
  inline Vectors::Vector<TPoint> &operator*=(double c)
  {
    unsigned int dims=TPoint::size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]*=c;

    return *this;
  };
  // multiply Vector by some value (enlarge vector)
  inline Vectors::Vector<TPoint> &operator*=(int c)
  {
    unsigned int dims=TPoint::size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]*=c;

    return *this;
  };

  // scalar multiply of a vectors
  inline double operator*(const Vectors::Vector<TPoint> &v) const
  {
    unsigned int dims=TPoint::size();
    double       res =0;
    for(unsigned int i=0; i<dims; ++i)
      res+=(*this)[i]*v[i];

    return res;
  };

  // normalizes Vector
  inline void normalize(void)
  {
    const double len =length();
    if(len==0)
      throw Vectors::Exception("Vector::normalize(): division by zero");

    unsigned int dims=TPoint::size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]/=len;
  };

  // reverse direction of vector
  inline void reverse(void)
  {
    unsigned int dims=TPoint::size();
    for(unsigned int i=0; i<dims; ++i)
      (*this)[i]*=-1;
  };
};


//
// some commonly used dimmentions sizes
//

template <typename TDimType>
class Vector2D: public Vectors::Vector< Vectors::Point2D<TDimType> >
{
public:
  Vector2D(const Vectors::Point2D<TDimType> &p1,
           const Vectors::Point2D<TDimType> &p2):
    Vector< Vectors::Point2D<TDimType> >(p1, p2)
  {
  };

  Vector2D(const TDimType &x, const TDimType &y)
  {
    (*this)[0]=x;
    (*this)[1]=y;
  };

  // returns angle (clock-wise) between this
  // vector and OY=[0;1] vector.
  inline double getOYangle(void) const
  {
    const Vectors::Vector2D<TDimType> oy(0,1);       // OY directed vector
    Vectors::Vector2D<TDimType>       dir=*this;     // helper reference
    dir.normalize();                  // both vector mus be normal

    const double scal=dir*oy;         // compute scalar multiply of vectors

    double arc=acos(scal);            // get arccos( scal )

    if(dir[0]<0)          // if vector is in 2nd or 3rd quater it
      arc=2*M_PI-arc;     // means it has more than 180degs

    return arc;           // return anlge in radians (clock-wise)
  };

  // returns angle (clock-wise) between this vector and
  // the one given as a parameter.
  inline double getAngle(const Vectors::Vector2D<TDimType> &v) const
  {
    // get angles between each of vectors and OY:
    const double at=getOYangle();
    const double av=v.getOYangle();
    const double da=av-at;              // delta

    if(da<0)              // this must be "reverse" angle
      return 2*M_PI+da;
    return da;            // this seems to be "regular" case (angle)
  };
};

template <typename TDimType>
class Vector3D: public Vectors::Vector< Vectors::Point3D<TDimType> >
{
public:
  Vector3D(const Vectors::Point3D<TDimType> &p1,
           const Vectors::Point3D<TDimType> &p2):
    Vector< Vectors::Point3D<TDimType> >(p1, p2)
  {
  };

  Vector3D(const TDimType &x, const TDimType &y, const TDimType &z)
  {
    *this[0]=x;
    *this[1]=y;
    *this[2]=z;
  };
};


//
// some commolny used spaces type
//

typedef Vector2D<double> Vector2DCont;
typedef Vector2D<int   > Vector2DDisc;

typedef Vector3D<double> Vector3DCont;
typedef Vector3D<int   > Vector3DDisc;


}; // namespace Vectors

#endif

