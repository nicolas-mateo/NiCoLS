#ifndef CONJUGATE_GRADIENTS_H
#define CONJUGATE_GRADIENTS_H

#include <nicols/linearAlgebra/linearSolvers/linearSolver.h>

class ConjugateGradients: public LinearSolver{
public:
  ConjugateGradients(int its, double tol):LinearSolver(its,tol){};
  void solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x) override;

};
#endif
