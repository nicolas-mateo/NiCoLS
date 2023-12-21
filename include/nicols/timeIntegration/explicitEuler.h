#ifndef EXPLICIT_EULER_H
#define EXPLICIT_EULER_H

#include <nicols/timeIntegration/timeIntegrator.h>

class ExplicitEuler: public TimeIntegrator{
public:
  ExplicitEuler(double dt, size_t steps):TimeIntegrator(dt,steps){};
  void step(std::unique_ptr<Matrix<double>>& A, const std::vector<double> b, std::vector<double>& x) override;
};

#endif
