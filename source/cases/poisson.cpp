#include <nicols/cases/poisson.h>
#include <nicols/linearAlgebra/linearSolvers/gaussSeidel.h>
#include <nicols/linearAlgebra/linearSolvers/sor.h>
#include <nicols/linearAlgebra/linearSolvers/cg.h>
#include <nicols/discretization/finiteDifference.h>

void Poisson::selectDataStruct(std::string matrixClass){
  if(matrixClass.compare("dense")==0)
    A=std::make_unique<DenseMatrix<double>>(_n,_n);
  if(matrixClass.compare("sparse")==0)
    A=std::make_unique<SparseMatrix<double>>(_n,_n);
  return;
}

void Poisson::selectSolver(std::string solverClass){
  if(solverClass.compare("Gauss-Seidel")==0)
    solver=std::make_unique<GaussSeidel>(10000,1E-6);
  if(solverClass.compare("SOR")==0)
    solver=std::make_unique<SOR>(10000,1E-6,1.7);
  if(solverClass.compare("CG")==0)
    solver=std::make_unique<ConjugateGradients>(10000,1E-6);
  return;

}

void Poisson::selectDiscretization(std::string scheme){
  if(scheme.compare("Finite Differences")==0)
    _scheme=std::make_unique<FiniteDifference>(_grid);
}

void Poisson::buildLinearSystem(){
  _scheme->assembleLinearSystem(A,b,_boundaries,source);
}

void Poisson::setBoundaryConditions(double x0,double x1){
  if(_dims!=1)
    throw std::invalid_argument("Can't build boundary conditions like this on a 2D Poisson.");

  _boundaries["West"]=DirichletBoundary(0.,x0);
  _boundaries["East"]=DirichletBoundary(1.,x1);
  return;
}

void Poisson::setBoundaryCondition(std::string name, DirichletBoundary boundary){
  if(name!="East"&&name!="North"&&name!="West"&&name!="South")
      throw std::invalid_argument("Boundary name must be either East, North, West or South.");
  _boundaries[name]=boundary;
}

Poisson::Poisson(int n, double x0, double x1):_dims(1),_n(n),_nX(n),_nY(1),b(n,0.),x(n,0.),_grid(n){
  setBoundaryConditions(x0,x1);
  selectDataStruct("dense");
  selectSolver("Gauss-Seidel");
  selectDiscretization("Finite Differences");
  buildLinearSystem();
  std::cout<<"Assembled linear system with n="<<_n<<" points, dx= "<<_grid.getDx()<<" step size."<<std::endl;
}

Poisson::Poisson(int nX, int nY):_dims(2),_n(nX*nY),_nX(nX),_nY(nY),b(_n,0.),x(_n,0.),_grid(nX,nY){
  selectDataStruct("sparse");
  selectSolver("CG");
  selectDiscretization("Finite Differences");
}

void Poisson::solve(int its, double tol){
  solver->setIterations(its);
  solver->setTolerance(tol);
  solver->solve(A,b,x);
  _grid.updateGrid(x);
  _grid.outputVTK("T.vtk");
  return;
}
