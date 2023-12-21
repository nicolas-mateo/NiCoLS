#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <functional>

#include <vtkCellData.h> //CommonDataModel
#include <vtkDoubleArray.h> //CommonCore
#include <vtkPointData.h> //CommonDataModel
#include <vtkPoints.h> //CommonDataModel
#include <vtkSmartPointer.h> //CommonDataModel
#include <vtkStructuredGrid.h> //CommonDataModel
#include <vtkStructuredGridWriter.h> //IOLegacy
#include <vtkTuple.h> //CommonMath

struct Cell{
  std::vector<double> position;
  double u;
};

class Grid {
public:
  Grid(int n);
  Grid(int nX, int nY);

  Cell operator[](int k);
  Cell& operator()(int i, int j);

  size_t getDims();
  size_t getSize();
  size_t getDims() const;
  size_t getSize() const;

  int getNx();
  int getNy();
  int getNx() const;
  int getNy() const;

  double getDx();
  double getDy();
  double getDx() const;
  double getDy() const;

  void setInitialCondition(std::function<double(double,double)> func);
  void updateGrid(const std::vector<double>& u_vals);
  void printGrid();
  void outputVTK(std::string outputName="output.vtk");

private:
  size_t dims;
  int nX;
  int nY;
  double length;
  double dx;
  double dy;
  std::vector<Cell> cells;
};
#endif
