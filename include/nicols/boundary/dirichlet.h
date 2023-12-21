#ifndef DIRICHLET_H
#define DIRICHLET_H

#include <nicols/boundary/boundary.h>

class DirichletBoundary: public Boundary {
public:
  DirichletBoundary():Boundary(){};
  DirichletBoundary(double x):Boundary(x){};
  DirichletBoundary(double x, double u):Boundary(x,u){};
  DirichletBoundary(double x, std::function<double(double)>func):Boundary(x,func){};
  double evaluateBoundary(double x) override;
  void setBoundaryFunction(std::function<double(double)>func) override;
  void setBoundaryValue(double u_b) override;
};

#endif // !DIRICHLET_H
