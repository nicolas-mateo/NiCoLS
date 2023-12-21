#ifndef TIME_INTEGRATOR_H
#define TIME_INTEGRATOR_H

#include <memory>
#include <nicols/linearAlgebra/matrix.h>

class TimeIntegrator{
public:
  TimeIntegrator(double dt,size_t steps):_dt(dt),_steps(steps){};
  virtual void step(std::unique_ptr<Matrix<double>>& A, const std::vector<double> b, std::vector<double>& x)=0;
protected:
  double _dt;
  size_t _steps;
};

#endif
