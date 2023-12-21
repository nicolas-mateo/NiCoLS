#include <nicols/cases/unsteadyDiffusion.h>
#include <nicols/discretization/finiteDifference.h>
#include <nicols/timeIntegration/explicitEuler.h>

void UnsteadyDiffusion::selectDataStruct(std::string matrixClass){
  if(matrixClass.compare("dense")==0)
    A=std::make_unique<DenseMatrix<double>>(_n,_n);
  if(matrixClass.compare("sparse")==0)
    A=std::make_unique<SparseMatrix<double>>(_n,_n);
  return;
}

void UnsteadyDiffusion::selectTimeStepper(std::string stepperClass){
  if(stepperClass.compare("explicit euler")==0)
    _timeIntegrator=std::make_unique<ExplicitEuler>(_dt,_steps);
  return;
}

void UnsteadyDiffusion::selectDiscretization(std::string scheme){
  if(scheme.compare("Finite Differences")==0)
    _scheme=std::make_unique<FiniteDifference>(_grid);
}

void UnsteadyDiffusion::setBoundaryCondition(std::string name, DirichletBoundary boundary){
  if(name!="East"&&name!="North"&&name!="West"&&name!="South")
      throw std::invalid_argument("Boundary name must be either East, North, West or South.");
  _boundaries[name]=boundary;
}

void UnsteadyDiffusion::buildLinearSystem(){
  _scheme->assembleLinearSystem(A,b,_boundaries, source);
}

UnsteadyDiffusion::UnsteadyDiffusion(int n):_n(n*n),_timestep(0),_dt(1E-3),_steps(1000),b(_n,0.),u(_n,0.),_grid(n,n){
  selectDataStruct("sparse");
  selectTimeStepper("explicit euler");
  selectDiscretization("Finite Differences");
}

UnsteadyDiffusion::UnsteadyDiffusion(int n, double dt, int steps):_n(n*n),_timestep(0),_dt(dt),_steps(steps),b(_n,0.),u(_n,0.),_grid(n,n){
  selectDataStruct("sparse");
  selectTimeStepper("explicit euler");
  selectDiscretization("Finite Differences");
}

void UnsteadyDiffusion::step(){
  _timeIntegrator->step(A,b,u);
  _timestep+=1;
}

void padTo(std::string &str, const size_t num, const char paddingChar = ' ')
{
    if(num > str.size())
        str.insert(0, num - str.size(), paddingChar);
}

void UnsteadyDiffusion::output(){
  std::string timestep = std::to_string(_timestep);
  padTo(timestep,10,'0');
  std::string filename = "U_"+timestep+".vtk";
  _grid.updateGrid(u);
  _grid.outputVTK(filename);
}
void UnsteadyDiffusion::setIC(double u0){
  std::fill(u.begin(),u.end(),u0);
  _grid.updateGrid(u);
  output();
}
