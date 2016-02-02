#ifndef COSMOGENIC_BOUNDS_H
#define COSMOGENIC_BOUNDS_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{

  template <class T>
  class Bounds{

    T lowEdge, upEdge;//may only be positive
    
  public:
    Bounds() = default;
    Bounds(T lowEdge, T upEdge);
    bool contains(const T& value) const;//excludes upEdge
    T getCenter() const;
    T getLowEdge() const;
    T getUpEdge() const;
    T getWidth() const;
    bool hasNegativeEdge() const;
    void setEdges(const T& lowEdge, const T& upEdge);
    Bounds<T>& shift(const T& value);
    
  };

  template <class T>
  Bounds<T>::Bounds(T lowEdge, T upEdge):lowEdge(lowEdge),upEdge(upEdge){
    
    if(upEdge < lowEdge){
      
      auto errorMessage = std::to_string(lowEdge)+" and "+std::to_string(upEdge)+" are not valid low and up bounds.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  bool Bounds<T>::contains(const T& value) const{
    
    if(value >= lowEdge && value < upEdge) return true;
    else return false;

  }

  template <class T>
  T Bounds<T>::getCenter() const{
    
    return (upEdge + lowEdge)*0.5;

  }

  template <class T>
  T Bounds<T>::getLowEdge() const{
    
    return lowEdge;

  }

  template <class T>
  T Bounds<T>::getUpEdge() const{
    
    return upEdge;

  }

  template <class T>
  T Bounds<T>::getWidth() const{
    
  return upEdge - lowEdge;

  }
  
  template <class T>
  bool Bounds<T>::hasNegativeEdge() const{
    
    return lowEdge < 0;

  }
  
  template <class T>
  void Bounds<T>::setEdges(const T& lowEdge, const T& upEdge){
    
    if(upEdge < lowEdge){
      
      auto errorMessage = std::to_string(lowEdge)+" and "+std::to_string(upEdge)+" are not valid low and up bounds.";
      throw std::invalid_argument(errorMessage);
      
    }
    else{
      
      this->lowEdge = lowEdge;
      this->upEdge = upEdge;
      
    }

  }

  template <class T>
  Bounds<T>& Bounds<T>::shift(const T& value){
    
    lowEdge += value;
    upEdge += value;
    
    return *this;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const Bounds<T>& bounds){
    
    auto formerPrecision = output.precision();
    output<<std::fixed;
    
    output<<"["<<std::setw(4)<<std::internal<<std::setprecision(1)<<bounds.getLowEdge()<<", "<<std::setw(4)<<std::internal<<bounds.getUpEdge()<<"]";
    output<<std::setprecision(formerPrecision);
    
    return output;
    
  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, Bounds<T>& bounds){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("(^"+number+")[:,]("+number+"$)");//start with a number :, seprator and end with another number
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) bounds.setEdges(std::stod(regexMatches[1]), std::stod(regexMatches[2]));
    else throw std::invalid_argument(token+" cannot be parsed to build bounds.");
    
    return input;
  
  }

  template <class T>
  Bounds<T> shift(Bounds<T> bounds, const T& value){//returns a bounds shifted from 'value'
    
    bounds.shift(value);
    return bounds;
    
  }
  
}

#endif