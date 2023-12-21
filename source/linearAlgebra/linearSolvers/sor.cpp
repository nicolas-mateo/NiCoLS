#include <nicols/linearAlgebra/linearSolvers/sor.h>

void SOR::setRelaxation(double lambda){
  _lambda=lambda;
  return;
}

void SOR::solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x) 
{
  if(b.size()!=x.size())
    throw std::invalid_argument("Vector sizes do not match.");
  std::cout<<"Solving with SOR"<<std::endl;
  
  double sum=0;
  int k=0;
  std::vector<double> interm=A->matVec(x);
  double delta=diff(interm,b);
  while(delta>(this->_tol*this->_tol) && (k<this->_its)){
    for(size_t i=0; i<x.size();++i){
      std::pair<size_t,size_t> columnRange = A->getColumnRange(i);
      for (size_t j=columnRange.first; j<columnRange.second; ++j) {
        sum+=A->operator()(i,j)*x.at(j);
      }
      x.at(i)=x.at(i)+(_lambda/A->operator()(i,i))*(b.at(i)-sum);
      sum=0.;
    }
    k++;
    if(k%100==0){
      interm = A->matVec(x);
      delta = diff(interm,b);
      std::cout<<"Iteration: "<<k<<" Delta: "<<delta<<std::endl;
    }
  }
  interm = A->matVec(x);
  delta = sqrt(diff(interm,b));
  std::cout<<"Solved in "<<k<<" iterations"<<std::endl;
  std::cout<<"With a delta of "<<delta<<std::endl;
  return;
}
