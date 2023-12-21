#include <nicols/nicols.h>
#include <nicols/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

/*
int main (int argc, char *argv[]) {

  if(argc>2)
    throw std::runtime_error("Too many arguments. Just choose case type!");

  if(argc==0){
    Poisson myPoisson(100,0.,1.);
    myPoisson.solve(1000000,1E-9);
    return 0;
  }

  std::string arg=argv[1];
  if(arg.compare("0")==0){
    Poisson myPoisson(100,0.,1.);
    myPoisson.solve(1000000,1E-9);
    return 0;
  }
  if(arg.compare("1")==0){
    Poisson myPoisson(200,200);
    DirichletBoundary west = DirichletBoundary(0.,0.25);
    DirichletBoundary east = DirichletBoundary(1.,0.5);
    DirichletBoundary south= DirichletBoundary(0.,0.0);
    DirichletBoundary north= DirichletBoundary(1.,1.0);
    myPoisson.setBoundaryCondition("West",west);
    myPoisson.setBoundaryCondition("East",east);
    myPoisson.setBoundaryCondition("North",north);
    myPoisson.setBoundaryCondition("South",south);
    myPoisson.buildLinearSystem();
    myPoisson.solve(100000,1E-6);

    return 0;
  }
  if(arg.compare("2")==0){
    UnsteadyDiffusion myUnsteadyDiffusion(20,1E-6,5000);
    DirichletBoundary west = DirichletBoundary(0.,0.25);
    DirichletBoundary east = DirichletBoundary(1.,0.5);
    DirichletBoundary south= DirichletBoundary(0.,0.0);
    DirichletBoundary north= DirichletBoundary(1.,1.0);
    myUnsteadyDiffusion.setBoundaryCondition("West",west);
    myUnsteadyDiffusion.setBoundaryCondition("East",east);
    myUnsteadyDiffusion.setBoundaryCondition("North",north);
    myUnsteadyDiffusion.setBoundaryCondition("South",south);
    myUnsteadyDiffusion.buildLinearSystem();
    myUnsteadyDiffusion.setIC(1.0);
    for(int i=1; i<=100;++i){
      for(int ii=1;ii<=1000;++ii){
        myUnsteadyDiffusion.step();
      }
      myUnsteadyDiffusion.output();
    }
  }
  if(arg.compare("help")==0){
    std::cout<<"This file solves the poisson problem in a 1D domain or a 2D square
domain."<<std::endl; std::cout<<"0: 1D Poisson with 100 points."<<std::endl; std::cout<<"1: 2D
Poisson with 200 points per dimension."<<std::endl; std::cout<<"2: 2D unsteady diffusion with 20
points per dimension and timestep of 1E-3 for 0.5 seconds"<<std::endl; std::cout<<"sparse: Check the
CSR Matrix data structure."<<std::endl;

    return 0;
  }
  if(arg.compare("sparse")==0){
    std::vector<double> x={1.,2.,3.};
    SparseMatrix<double> A(3,3);
    SparseMatrix<double> B(3,3);
    int k{0};
    for(size_t i=0;i<3;++i){
      for(size_t j=0;j<3;++j){
        if(i==j)
          A.set(i,j,1.);
        B.set(i,j,++k);
      }
    }
    A.assemble();
    B.assemble();
    A.printMatrix();
    B.printMatrix();
    return 0;
  }
}
*/

int main(int argc, char** argv) {
  cxxopts::Options options(*argv, "A program to welcome the world!");

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("p,poisson", "Run a 1D steady Poisson case")
    ("i,iterative", "Run a 2D steady Poisson case solved with CG")
    ("u,unsteady", "Run an unsteady diffusion case with explicit Euler method")
    ("s,sparse", "Run an example of sparse matrices")
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "NiCoLS, version " << NICOLS_VERSION << std::endl;
    return 0;
  }

  if (result["poisson"].as<bool>()) {
    Poisson myPoisson(100, 0., 1.);
    myPoisson.solve(1000000, 1E-9);
    return 0;
  }

  if (result["iterative"].as<bool>()) {
    Poisson myPoisson(200, 200);
    DirichletBoundary west = DirichletBoundary(0., 0.25);
    DirichletBoundary east = DirichletBoundary(1., 0.5);
    DirichletBoundary south = DirichletBoundary(0., 0.0);
    DirichletBoundary north = DirichletBoundary(1., 1.0);
    myPoisson.setBoundaryCondition("West", west);
    myPoisson.setBoundaryCondition("East", east);
    myPoisson.setBoundaryCondition("North", north);
    myPoisson.setBoundaryCondition("South", south);
    myPoisson.buildLinearSystem();
    myPoisson.solve(100000, 1E-6);

    return 0;
  }

  if (result["unsteady"].as<bool>()) {
    UnsteadyDiffusion myUnsteadyDiffusion(20, 1E-6, 5000);
    DirichletBoundary west = DirichletBoundary(0., 0.25);
    DirichletBoundary east = DirichletBoundary(1., 0.5);
    DirichletBoundary south = DirichletBoundary(0., 0.0);
    DirichletBoundary north = DirichletBoundary(1., 1.0);
    myUnsteadyDiffusion.setBoundaryCondition("West", west);
    myUnsteadyDiffusion.setBoundaryCondition("East", east);
    myUnsteadyDiffusion.setBoundaryCondition("North", north);
    myUnsteadyDiffusion.setBoundaryCondition("South", south);
    myUnsteadyDiffusion.buildLinearSystem();
    myUnsteadyDiffusion.setIC(1.0);
    for (int i = 1; i <= 100; ++i) {
      for (int ii = 1; ii <= 1000; ++ii) {
        myUnsteadyDiffusion.step();
      }
      myUnsteadyDiffusion.output();
    }
    return 0;
  }
  return 0;
}
