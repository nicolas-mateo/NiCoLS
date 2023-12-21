#ifndef DISCRETIZATION_H
#define DISCRETIZATION_H

#include <memory>
#include <nicols/linearAlgebra/nicols_la.h>
#include <nicols/boundary/nicols_bc.h>
#include <nicols/grid/nicols_grid.h>

class Discretization{
public:
  Discretization(const Grid& grid):_dims(grid.getDims()),_n(grid.getSize()),_nX(grid.getNx()),_nY(grid.getNy()),dX(grid.getDx()),dY(grid.getDy()){};
  virtual void assembleLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source)=0;

protected:
  size_t _dims;
  size_t _n;
  size_t _nX;
  size_t _nY;
  double dX;
  double dY;
};

#endif

