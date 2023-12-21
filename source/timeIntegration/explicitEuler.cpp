#include <nicols/timeIntegration/explicitEuler.h>

void ExplicitEuler::step(std::unique_ptr<Matrix<double>>& A, const std::vector<double> b, std::vector<double>& x){
  std::vector<double> dx = A->matVec(x) + b;
  dx = scalarMult(_dt,dx);
  x=x+dx;
  return;
}
