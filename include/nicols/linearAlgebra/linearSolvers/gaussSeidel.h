#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

#include <nicols/linearAlgebra/linearSolvers/linearSolver.h>

class GaussSeidel: public LinearSolver{
public:
  GaussSeidel(int its, double tol):LinearSolver(its,tol){};
  void solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x) override;
};

#endif
