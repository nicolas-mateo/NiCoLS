#include <nicols/discretization/finiteDifference.h>

void FiniteDifference::build1DLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source){

  double dX2=dX*dX;
  double a_ii=-2./dX2;
  double a_ij=1/dX2;
  


  b.at(0)+=boundaries["West"].evaluateBoundary(0.)/dX2;
  if(source)
    b.at(0)+=source(0,0);
  
  A->set(0,0,a_ii);
  A->set(0,1,a_ij);
  for(size_t i=1;i<_n-1;++i){
    A->set(i,i-1,a_ij);
    A->set(i,i,a_ii);
    A->set(i,i+1,a_ij);
  }
  b.at(_n-1)+=boundaries["East"].evaluateBoundary(1.)/dX2;
  A->set(_n-1,_n-2,a_ij);
  A->set(_n-1,_n-1,a_ii);
}

void FiniteDifference::build2DLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source){

  double dX2 = dX*dX;
  double dY2 = dY*dY;
  double a_ii = -2.*(dX2 + dY2)/(dX2*dY2);
  double a_i = 1./dX2;
  double a_j = 1./dY2;

  if(source)
    b.at(0)+=source(0,0);

  for(size_t i=0;i<_n;++i){
    size_t pos = i%_nX;
    if(pos!=0){
      A->set(i,i-1,a_i);
    }else {
      b.at(i)+=boundaries["West"].evaluateBoundary(0.)/dX2;
    }
    if(pos!=_nX-1){
      A->set(i,i+1,a_i);
    }else {
      b.at(i)+=boundaries["East"].evaluateBoundary(1.0)/dX2;
    }
    if(i>_nX-1){
      A->set(i,i-_nX,a_j);
    }else {
      b.at(i)+=boundaries["South"].evaluateBoundary(0.)/dY2;
    }
    if(i<_n-_nX){
      A->set(i,i+_nX,a_j);
    }else {
      b.at(i)+=boundaries["North"].evaluateBoundary(1.)/dY2;
    }
    A->set(i,i,a_ii);
  }
  A->assemble();
  return;
}

void FiniteDifference::assembleLinearSystem(std::unique_ptr<Matrix<double>>& A, std::vector<double>& b, std::map<std::string, DirichletBoundary> boundaries, std::function<double(double, double)> source){
  if(_dims==1)
    build1DLinearSystem(A,b,boundaries, source);
  if(_dims==2)
    build2DLinearSystem(A, b,boundaries, source);
  return;
}
