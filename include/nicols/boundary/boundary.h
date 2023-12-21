#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <functional>
#include <stdexcept>


class Boundary {
public:
  Boundary():_position(-1.){};
  Boundary(double x):_position(x){};
  Boundary(double x, double u):_position(x),_u(u),isValueSet(true){};
  Boundary(double x, std::function<double(double)>func):_position(x),isValueSet(false),_uX(func){};
  virtual double evaluateBoundary(double x)=0; 
  virtual void setBoundaryFunction(std::function<double(double)> func)=0;
  virtual void setBoundaryValue(double u_b)=0;

protected:
  double _position;
  double _u;
  bool isValueSet=false;
  std::function<double(double)> _uX;
};

#endif // !BOUNDARY_H
