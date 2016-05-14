#ifndef COSMOGENIC_CHIMNEY_VETO_H
#define COSMOGENIC_CHIMNEY_VETO_H

#include <iomanip>
#include <stdexcept>
#include <regex>
#include "Cosmogenic/Veto.hpp"

namespace CosmogenicHunter{
  
  template <class T>
  class ChimneyVeto : public Veto<T>{
    
    T minChimneyInconsistencyRatio;
    
  public:
    ChimneyVeto();
    explicit ChimneyVeto(T minChimneyInconsistencyRatio);
    T getMinChimneyInconsistencyRatio() const;
    void setMinChimneyInconsistencyRatio(T minChimneyInconsistencyRatio);
    bool veto(const Single<T>& single) const;
    bool veto(const CandidatePair<T>& candidatePair) const;
    std::unique_ptr<Veto<T>> clone() const;
    void print(std::ostream& output) const;
    
  };
  
  template <class T>
  ChimneyVeto<T>::ChimneyVeto()
  :ChimneyVeto<T>(0){
    
  }
  
  template <class T>
  ChimneyVeto<T>::ChimneyVeto(T minChimneyInconsistencyRatio)
  :Veto<T>("ChimneyVeto"),minChimneyInconsistencyRatio(minChimneyInconsistencyRatio){
    
    if(minChimneyInconsistencyRatio < 0) throw std::invalid_argument(std::to_string(minChimneyInconsistencyRatio)+" is not a valid chimney inconsistency ratio.");
    
  }

  template <class T>
  T ChimneyVeto<T>::getMinChimneyInconsistencyRatio() const{
    
    return minChimneyInconsistencyRatio;

  }

  template <class T>
  void ChimneyVeto<T>::setMinChimneyInconsistencyRatio(T minChimneyInconsistencyRatio){
    
    if(minChimneyInconsistencyRatio >= 0) this->minChimneyInconsistencyRatio = minChimneyInconsistencyRatio;
    else throw std::invalid_argument(std::to_string(minChimneyInconsistencyRatio)+" is not a valid chimney inconsistency ratio.");

  }
  
  template <class T>
  bool ChimneyVeto<T>::veto(const Single<T>& single) const{

    return 2*single.getChimneyInconsistencyRatio() < minChimneyInconsistencyRatio;

  }
  
  template <class T>
  bool ChimneyVeto<T>::veto(const CandidatePair<T>& candidatePair) const{

    return candidatePair.getPrompt().getChimneyInconsistencyRatio() + candidatePair.getDelayed().getChimneyInconsistencyRatio() < minChimneyInconsistencyRatio;//the inconsistency at the chimney is too low (likely to be at the chimney) to be anything but a stopping muon

  }
  
  template <class T>
  std::unique_ptr<Veto<T>> ChimneyVeto<T>::clone() const{

    return std::make_unique<ChimneyVeto<T>>(*this);

  }
  
  template <class T>
  void ChimneyVeto<T>::print(std::ostream& output) const{
    
    output<<std::setw(28)<<std::left<<"Min chimney inconsistency"<<": "<<std::setw(6)<<std::right<<minChimneyInconsistencyRatio;

  }

  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const ChimneyVeto<T>& chimneyVeto){

    chimneyVeto.print(output);  
    return output;

  }
  
  
  template <class T>
  std::istream& operator>>(std::istream& input, ChimneyVeto<T>& chimneyVeto){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("^("+number+")$");
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) chimneyVeto.setMinChimneyInconsistencyRatio(std::stod(regexMatches[1]));
    else throw std::invalid_argument(token+" cannot be parsed to build chimney veto.");
    
    return input;
  
  }
  
}

#endif