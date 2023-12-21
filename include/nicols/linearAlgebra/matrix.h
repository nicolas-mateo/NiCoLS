#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <math.h>
#include <vector>
#include <map>

template<typename T>
T dot(const std::vector<T>& a,const std::vector<T>& b){
  if(a.size()!=b.size()){
    std::cout<<"Vector sizes do not match\n";
    return 0;
  }
  T sum=0;
  for(size_t i=0;i<a.size();++i){
    sum+=a[i]*b[i];
  }
  return sum;
}

template<typename T>
T diff(const std::vector<T>& a, const std::vector<T>& b){
  T sum =0;
  for(size_t i=0;i<a.size();++i){
    sum+=(a.at(i)-b.at(i))*(a.at(i)-b.at(i));
  }
  return sum;
}

template <typename T>
void printVector(const std::vector<T>& a){
  for (T element:a){
    std::cout<<element<<" ";
  }
  std::cout<<std::endl;
  return;
}

template <typename T>
std::vector<T> scalarMult(T a, std::vector<T> x){
  std::vector result(x.size(),0.);
  for(size_t i=0;i<x.size();++i)
    result.at(i)+=x.at(i)*a;
  return result;
}

template<typename T>
std::vector<T> operator+(std::vector<T>a,std::vector<T>b){
  if(a.size()!=b.size())
    throw std::runtime_error("Vector sizes do not match.");
  for(size_t i=0;i<a.size();++i)
    a.at(i)+=b.at(i);
  return a;
}

template<typename T>
std::vector<T> operator-(std::vector<T>a,std::vector<T>b){
  if(a.size()!=b.size())
    throw std::runtime_error("Vector sizes do not match.");
  for(size_t i=0;i<a.size();++i)
    a.at(i)-=b.at(i);
  return a;
}


template<typename T>
class Matrix{
public:
  virtual T operator()(size_t i, size_t j) const;
  virtual T operator()(size_t i, size_t j);
  virtual void set(size_t i, size_t j, T val);
  virtual void assemble();
  virtual std::vector<T> matVec(const std::vector<T>& vec) const;
  virtual std::vector<T> matVec(std::vector<T>& vec) const;
  virtual std::vector<T> matVec(std::vector<T>& vec);
  virtual std::pair<size_t, size_t> getColumnRange(size_t i) const;
};

template<typename T>
class DenseMatrix: public Matrix<T>{
public:
  DenseMatrix<T>(int ni,int nj):_ni(ni),_nj(nj),_data(std::vector<T>(ni*nj,0.)){ };
  DenseMatrix<T>(int ni,int nj, T k):_ni(ni),_nj(nj),_data(std::vector<T>(ni*nj,k)){ };
  DenseMatrix<T>(int ni, int nj, const std::vector<T>& data):_ni(ni),_nj(nj),_data(data){};

  T operator()(size_t i,size_t j) const override {return _data.at(i*_nj+j);}
  T operator()(size_t i,size_t j) override {return _data.at(i*_nj+j);}

  void set(size_t i, size_t j, T val) override {
    _data.at(i*_nj+j)=val;
    return;
  };

  void assemble() override {
    return;
  };

  std::vector<T> matVec(const std::vector<T>& vec) const override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0; i<_ni; ++i){
      for(size_t j=0; j<vec.size();++j){
        result.at(i)+=vec.at(j)*operator()(i,j);
      }
    }
    return result;
  };

  std::vector<T> matVec(std::vector<T>& vec) const override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0; i<_ni; ++i){
      for(size_t j=0; j<vec.size();++j){
        result.at(i)+=vec.at(j)*operator()(i,j);
      }
    }
    return result;
  };

  std::vector<T> matVec(std::vector<T>& vec) override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0; i<_ni; ++i){
      for(size_t j=0; j<vec.size();++j){
        result.at(i)+=vec.at(j)*operator()(i,j);
      }
    }
    return result;
  };

  std::pair<size_t,size_t> getColumnRange(size_t i) const override {
    if(i>=_ni)
      throw std::runtime_error("Row is out of matrix range.");
    return std::make_pair(0,_nj);
  };

  void printMatrix(){
    for(size_t i=0;i<_ni;++i){
      for(size_t j=0; j<_nj; ++j){
        std::cout<< operator()(i,j) << " ";
      }
      std::cout<<std::endl;
    }
  };

private:
  const size_t _ni;
  const size_t _nj;
  std::vector<T> _data;
};

template<typename T>
class SparseMatrix: public Matrix<T>{
public:
  SparseMatrix<T>(int ni, int nj):_ni(ni),_nj(nj),k(0),Assembled(false){};
  void set(size_t i, size_t j, T val) override {
    if(Assembled)
      throw std::runtime_error("Matrix is already assembled, can't set new values.");
    if(i>=_ni || j>=_nj)
      throw std::out_of_range("Element is out of matrix range.");
    if(val==0.)
      throw std::runtime_error("Sparse matrix format does not take 0 values. Use DenseMatrix");

    _map[std::make_pair(i,j)]=val;
  };

  void printMatrix(){
    if(Assembled){
      for(size_t i=0;i<_ni;++i){
        for(int K=_start[i];K<_start[i+1];K++){
          std::cout<<"A("<<i<<", "<<_j[K]<<") = "<<_data[K]<<std::endl;
        }
      }
    }else{
      for(const auto& [ij,value]:_map)
        std::cout<<"A("<<ij.first<<", "<<ij.second<<") = "<<value<<std::endl;
    }
  };

  void assemble() override {
    if(Assembled)
      throw std::runtime_error("Matrix is already assembled, can't re-assemble.");
    if(_map.empty())
      throw std::runtime_error("Matrix is empty, can't assemble an empty matrix.");
    int lastRow=-1;
    for(const auto& [ij,value]:_map){
      if(ij.first!=lastRow){
        _start.push_back(k);
        lastRow=ij.first;
      }
      _data.push_back(value);
      _j.push_back(ij.second);
      k++;
    }
    _start.push_back(_data.size());
    _map.clear();
    Assembled=true;
    return;
  };

  T operator()(size_t i, size_t j) const override {
    if(i>=_ni || j>=_nj)
      throw std::out_of_range("Element is out of matrix range.");
    if(!Assembled)
      throw std::runtime_error("Index into a non-assembled matrix.");
    for(int K=_start[i];K<_start[i+1];++K){
      if(_j[K]==j)
        return _data[K];
    }
    return 0;
  };

  T operator()(size_t i, size_t j) override {
    if(i>=_ni || j>=_nj)
      throw std::out_of_range("Element is out of matrix range.");
    if(!Assembled)
      throw std::runtime_error("Cannot index into a non-assembled matrix.");
    for(int K=_start[i];K<_start[i+1];++K){
      if(_j[K]==j)
        return _data[K];
    }
    return 0;
  };

  std::vector<T> matVec(const std::vector<T>& vec) const override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0;i<_ni;++i){
      for(int K=_start[i];K<_start[i+1];++K){
        result[i]+=_data[K]*vec[_j[K]];
      }
    }
    return result;
  };

  std::vector<T> matVec(std::vector<T>& vec) const override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0;i<_ni;++i){
      for(int K=_start[i];K<_start[i+1];++K){
        result[i]+=_data[K]*vec[_j[K]];
      }
    }
    return result;
  };

  std::vector<T> matVec(std::vector<T>& vec) override {
    if(vec.size()!=_nj)
      throw std::runtime_error("Matrix column size "+std::to_string(_nj)+" and vector size "+std::to_string(vec.size())+" do not match!");
    std::vector<T> result(_ni,0);
    for(size_t i=0;i<_ni;++i){
      for(int K=_start[i];K<_start[i+1];++K){
        result[i]+=_data[K]*vec[_j[K]];
      }
    }
    return result;
  };

  std::pair<size_t, size_t> getColumnRange(size_t i) const override{
    if(!Assembled)
      throw std::runtime_error("Sparse matrix is not assembled.");
    if(i>=_ni)
      throw std::runtime_error("Row is out of matrix range.");
    return std::make_pair(_j[_start[i]],_j[_start[i+1]-1]+1);
  }

private:
  const size_t _ni;
  const size_t _nj;
  int k;
  bool Assembled;
  std::map<std::pair<int,int>,T> _map;
  std::vector<size_t> _j;
  std::vector<int> _start;
  std::vector<T> _data;
};
#endif
