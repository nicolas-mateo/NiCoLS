#ifndef SOR_H
#define SOR_H

#include <nicols/linearAlgebra/linearSolvers/linearSolver.h>

class SOR: public LinearSolver{
public:
  SOR(int its, double tol, double lambda):LinearSolver(its,tol),_lambda(lambda){};
  void setRelaxation(double lambda);
  void solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x) override;
private:
  double _lambda;
};

#endif
