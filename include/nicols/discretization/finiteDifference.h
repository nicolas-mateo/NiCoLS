#ifndef FINITE_DIFFERENCE_H
#define FINITE_DIFFERENCE_H

#include <nicols/discretization/discretization.h>

class FiniteDifference: public Discretization{
public:
  FiniteDifference(const Grid& grid):Discretization(grid){};
  void assembleLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source) override;
private:
  void build1DLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source);
  void build2DLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source);

};



#endif // !FINITE_DIFFERENCE_H
