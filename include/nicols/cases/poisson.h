#ifndef POISSON_H
#define POISSON_H

#include <nicols/discretization/nicols_disc.h>
#include <nicols/grid/nicols_grid.h>
#include <nicols/boundary/nicols_bc.h>
#include <nicols/linearAlgebra/nicols_la.h>

class Poisson{
public:
  Poisson(int n, double x0, double x1);
  Poisson(int nX, int nY);
  void selectDataStruct(std::string matrixClass);
  void selectSolver(std::string solverClass);
  void selectDiscretization(std::string scheme);
  void buildLinearSystem();
  void setBoundaryConditions(double x0, double x1);
  void setBoundaryCondition(std::string name, DirichletBoundary boundary);
  void setRHS(); 
  void solve(int its=10000,double tol=1E-6);

private:
  int _dims;
  int _n;
  int _nX;
  int _nY;
  std::unique_ptr<Matrix<double>> A;
  std::vector<double> b;
  std::vector<double> x;
  std::unique_ptr<Discretization> _scheme;
  Grid _grid;
  std::map<std::string, DirichletBoundary> _boundaries;
  std::unique_ptr<LinearSolver> solver;
  std::function<double(double,double)> source=0;
};

#endif // !DEBUG
