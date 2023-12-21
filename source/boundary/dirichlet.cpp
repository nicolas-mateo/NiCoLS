#include <nicols/boundary/dirichlet.h>
#include <stdexcept>

void DirichletBoundary::setBoundaryValue(double u_b){
  if(_uX)
    throw std::invalid_argument("Can't set boundary value if boundary has a set function.");
  _u=u_b;
  isValueSet=true;
  return;
}

void DirichletBoundary::setBoundaryFunction(std::function<double(double)>func){
  if(isValueSet)
    throw std::invalid_argument("Can't set boundary function if boundary has a set value.");
  _uX=func;
  return;
}

double DirichletBoundary::evaluateBoundary(double x){
  if(_uX)
    return _uX(x);
  return _u;
}
