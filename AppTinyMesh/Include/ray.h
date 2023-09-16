#ifndef __Ray__
#define __Ray__

#include "mathematics.h"

// Ray class
class Ray
{
public:
    Vector _origin; //!< Origin of the ray.
    Vector _direction; //!< Direction.

public:
  //! Empty.
  Ray() {}
  explicit Ray(const Vector&, const Vector&);
  //! Empty.
  ~Ray() {}

  Ray Reflect(const Vector&, const Vector&);

  Vector operator()(double) const;

  // Functions to access Vector class components
  Vector Origin() const;
  Vector Direction() const;

  friend std::ostream& operator<<(std::ostream&, const Ray&);
};

/*!
\brief Creates a ray.

The direction should be unit:
\code
Ray ray(Vector(0.0,0.0,0.0),Normalized(Vector(2.0,-1.0,3.0)));
\endcode
\param p Origin.
\param d Direction (should be unit vector).
*/
inline Ray::Ray(const Vector& p, const Vector& d)
{
  _origin = p;
  _direction = d;
}

/*!
\brief Return the origin of the ray.
*/
inline Vector Ray::Origin() const
{
  return _origin;
}

/*!
\brief Return the direction of the ray.
*/
inline Vector Ray::Direction() const
{
  return _direction;
}

/*!
\brief Computes the location of a vertex along the ray.
\param t Parameter.
*/
inline Vector Ray::operator()(double t) const
{
  return _origin + t * _direction;
}

#endif
