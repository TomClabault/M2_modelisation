// Implicits

#pragma once

#include <functional>
#include <iostream>

#include "SDF/SDF.h"
#include "mesh.h"

class AnalyticScalarField
{
protected:
public:
  AnalyticScalarField();
  //AnalyticScalarField(SDF sdf);
  virtual double Value(const Vector&) const;
  static Vector Gradient(const Vector& p, const std::function<float(const Vector& point)>& value_function);

  // Normal
  static Vector Normal(const Vector& p, const std::function<float(const Vector& point)>& value_function);

  // Dichotomy
  static Vector Dichotomy(Vector, Vector, double, double, double, const std::function<float(const Vector& point)>& value_function, const double& epsilon = 1.0e-4);

  virtual void Polygonize(int n, Mesh& g, const Box& box, const double& epsilon = 1e-4) const;
  static void Polygonize_from_function(std::function<float(const Vector& point)> value_function, int n, Mesh& g, const Box& box, const double& epsilon = 1e-4);
protected:
  static const double Epsilon; //!< Epsilon value for partial derivatives
protected:
  static int TriangleTable[256][16]; //!< Two dimensionnal array storing the straddling edges for every marching cubes configuration.
  static int edgeTable[256];    //!< Array storing straddling edges for every marching cubes configuration.
};
