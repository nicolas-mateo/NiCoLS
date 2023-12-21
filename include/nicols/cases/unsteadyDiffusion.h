#ifndef UNSTEADY_DIFFUSION_H
#define UNSTEADY_DIFFUSION_H

#include <nicols/grid/grid.h>
#include <nicols/boundary/dirichlet.h>
#include <nicols/linearAlgebra/matrix.h>
#include <nicols/timeIntegration/timeIntegrator.h>
#include <nicols/discretization/discretization.h>

class UnsteadyDiffusion{
public:
  UnsteadyDiffusion(int n);
  UnsteadyDiffusion(int n, double dt, int steps);
  void selectDataStruct(std::string matrixClass);
  void selectTimeStepper(std::string stepperClass);
  void selectDiscretization(std::string scheme);
  void buildLinearSystem();
  void setBoundaryCondition(std::string name, DirichletBoundary boundary);
  void step();
  void output();
  void setIC(double u0);

private:
  int _n;
  int _timestep;
  double _dt;
  int _steps;
  std::unique_ptr<Matrix<double>> A;
  std::vector<double> b;
  std::vector<double> u;
  std::unique_ptr<Discretization> _scheme;
  Grid _grid;
  std::map<std::string, DirichletBoundary> _boundaries;
  std::unique_ptr<TimeIntegrator> _timeIntegrator;
  std::function<double(double,double)> source=0;
};

#endif
