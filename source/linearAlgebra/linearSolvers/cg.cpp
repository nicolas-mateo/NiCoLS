#include <nicols/linearAlgebra/linearSolvers/cg.h>

void ConjugateGradients::solve(std::unique_ptr<Matrix<double>>& A, const std::vector<double>& b, std::vector<double>& x)
{
  if(b.size()!=x.size())
    throw std::invalid_argument("Vector sizes do not match.");

  std::vector<double> r = b - A->matVec(x);
  std::vector<double> d = r;
  std::vector<double> Ad = A->matVec(d);
  int k=0;
  double norm2= dot(r,r);
  double newNorm2=0;
  double alpha = norm2/(dot(d,Ad));
  double beta = 0;
  while(norm2>(this->_tol * this->_tol)){
    x=x+scalarMult(alpha,d);
    r=r+scalarMult(-1.*alpha,Ad);
    newNorm2=dot(r,r);
    beta = newNorm2/norm2;
    d = r + scalarMult(beta, d);
    Ad = A->matVec(d);
    norm2=newNorm2;
    alpha = norm2/dot(d,Ad);
    k++;
    std::cout<<"Iteration: "<<k<<" Squared Norm:"<<norm2<<std::endl;
  }
}
