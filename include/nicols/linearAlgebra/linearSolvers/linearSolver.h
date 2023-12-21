#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include <memory>
#include <nicols/linearAlgebra/matrix.h>

class LinearSolver{
  /*
  Base class for solver
  @param x initial solution
  @param A matrix to solve
  @param b right-hand side vector
  @param its total maximum iterations
  @param tol tolerance for solution    
  */
public:
  LinearSolver(int its, double tol):_its(its), _tol(tol){};
  virtual void solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x)=0;
  void setIterations(int its){_its=its;};
  void setTolerance(double tol){_tol=tol;};
protected:
  int _its;
  double _tol;
};
#endif
