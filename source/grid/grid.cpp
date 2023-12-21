#include <nicols/grid/grid.h>

Grid::Grid(int n):dims(1),nX(n),nY(1),length(1.0),dx(length/(n+1)),dy(1.0){
  cells.resize(n);
  for(int i=0;i<n;++i){
    cells[i].position={(i+1)*dx,0.5};
    cells[i].u=0;
  }
}

Grid::Grid(int nX, int nY):dims(2),nX(nX),nY(nY),length(1.0),dx(length/(nX+1)),dy(length/(nY+1)){
  cells.resize(nX*nY);
  for(int i=0;i<nX;++i){
    for(int j=0;j<nY;++j){
      int k=i+j*nX;
      cells[k].position={(i+1)*dx,(j+1)*dy};
      cells[k].u=0;
    }
  }
}

Cell Grid::operator[](int k){return cells[k];}
Cell& Grid::operator()(int i, int j) {return cells.at(i*nY+j);}

size_t Grid::getDims(){return dims;}
size_t Grid::getSize(){return nX*nY;}
size_t Grid::getDims() const {return dims;}
size_t Grid::getSize() const {return nX*nY;}

double Grid::getDx(){return dx;}
double Grid::getDy(){return dy;}
double Grid::getDx() const {return dx;}
double Grid::getDy() const {return dy;}

int Grid::getNx(){return nX;}
int Grid::getNy(){return nY;}
int Grid::getNx() const {return nX;}
int Grid::getNy() const {return nY;}

void Grid::setInitialCondition(std::function<double(double,double)> func){
  for(Cell& cell:cells){
    cell.u=func(cell.position[0],cell.position[1]);
  }
}

void Grid::updateGrid(const std::vector<double>& u_vals){
  if(u_vals.size() != cells.size()){
    throw std::runtime_error("Can't update grid from vector, sizes do not match.\n");
  }
  for(int i=0;i<nX;++i){
    for (int j=0; j<nY; ++j) {
      int k=i*nY+j;
      operator()(i,j).u=u_vals[k];
    }
  }
}

void Grid::printGrid(){
  for(auto cell:cells){
    std::cout << "Position: (" << cell.position[0] << "," << cell.position[1] <<"); u: " << cell.u << std::endl;
  }
}

void Grid::outputVTK(std::string outputName){
  double z=0.0;
  vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

  for (auto cell:cells){
    points->InsertNextPoint(cell.position[0],cell.position[1],z);
  }

  structuredGrid->SetDimensions(nX,nY,1);
  structuredGrid->SetPoints(points);
  vtkSmartPointer<vtkDoubleArray> u = vtkSmartPointer<vtkDoubleArray>::New();

  u->SetName("temperature");
  u->SetNumberOfComponents(1);
  for(auto cell:cells){
    double u_i = cell.u;
    u->InsertNextTuple(&u_i);
  }
  structuredGrid->GetPointData()->AddArray(u);
  vtkSmartPointer<vtkStructuredGridWriter> writer = vtkSmartPointer<vtkStructuredGridWriter>::New();
  writer->SetFileName(outputName.c_str());
  writer->SetInputData(structuredGrid);
  writer->Write();
}
