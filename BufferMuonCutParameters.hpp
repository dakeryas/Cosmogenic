#ifndef COSMOGENIC_BUFFER_MUON_CUT_PARAMETERS_H
#define COSMOGENIC_BUFFER_MUON_CUT_PARAMETERS_H

#include <iomanip>
#include <stdexcept>
#include <regex>
#include "Cosmogenic/Veto.hpp"

namespace CosmogenicHunter{
  
  template <class T>
  class BufferMuonCutParameters : public Veto<T>{
    
    T constant;
    T exponent;
    
  public:
    BufferMuonCutParameters();
    BufferMuonCutParameters(T constant, T exponent);
    T getConstant() const;
    T getExponent() const;
    void setConstant(T constant);
    void setExponent(T exponent);
    void setParameters(T constant, T exponent);
    bool veto(const Single<T>& single) const;
    bool veto(const CandidatePair<T>& candidatePair) const;
    std::unique_ptr<Veto<T>> clone() const;
    void print(std::ostream& output) const;
    
  };
  
  template <class T>
  BufferMuonCutParameters<T>::BufferMuonCutParameters()
  :BufferMuonCutParameters<T>(std::numeric_limits<T>::max(), 0){
    
  }
  
  template <class T>
  BufferMuonCutParameters<T>::BufferMuonCutParameters(T constant, T exponent)
  :Veto<T>("BufferMuonVeto"),constant(constant),exponent(exponent){
    
    if(constant < 0 || exponent < 0 ){
      
      auto errorMessage = std::to_string(constant)+" MeV and "+std::to_string(exponent)+" are invalid buffer muon cut parameters.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T BufferMuonCutParameters<T>::getConstant() const{
    
    return constant;

  }

  template <class T>
  T BufferMuonCutParameters<T>::getExponent() const{
    
    return exponent;

  }
  
  template <class T>
  void BufferMuonCutParameters<T>::setConstant(T constant){
    
    if(constant >= 0) this->constant = constant;
    else throw std::invalid_argument(std::to_string(constant)+"MeV^"+std::to_string(exponent)+" is not a valid constant for the buffer muon cut.");

  }
  
  template <class T>
  void BufferMuonCutParameters<T>::setExponent(T exponent){
    
    if(exponent >= 0) this->exponent = exponent;
    else throw std::invalid_argument(std::to_string(exponent)+" is not a valid exponent for the buffer muon cut.");

  }
  
  template <class T>
  void BufferMuonCutParameters<T>::setParameters(T constant, T exponent){

    setConstant(constant);
    setExponent(exponent);

  }
  
  template <class T>
  bool BufferMuonCutParameters<T>::veto(const Single<T>& single) const{

    return single.getChargeInformation().getRatio() > constant / std::pow(single.getVisibleEnergy(), exponent); // the charge ratio is too high for such a small energy

  }
  
  template <class T>
  bool BufferMuonCutParameters<T>::veto(const CandidatePair<T>& candidatePair) const{

    return veto(candidatePair.getPrompt());

  }
  
  template <class T>
  std::unique_ptr<Veto<T>> BufferMuonCutParameters<T>::clone() const{

    return std::make_unique<BufferMuonCutParameters<T>>(*this);

  }
  
  template <class T>
  void BufferMuonCutParameters<T>::print(std::ostream& output) const{
    
    int labelColumnWidth = 8;
    int dataColumnWidth = 6;
    
    output<<std::setw(labelColumnWidth)<<std::left<<"Constant"<<": "<<std::setw(dataColumnWidth)<<std::right<<constant<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Exponent"<<": "<<std::setw(dataColumnWidth)<<std::right<<exponent;

  }

  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const BufferMuonCutParameters<T>& bufferMuonCutParameters){

    bufferMuonCutParameters.print(output);  
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, BufferMuonCutParameters<T>& bufferMuonCutParameters){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("(^"+number+")[:,]("+number+"$)");//start with a number :, seprator and end with another number
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) bufferMuonCutParameters.setParameters(std::stod(regexMatches[1]), std::stod(regexMatches[2]));
    else throw std::invalid_argument(token+" cannot be parsed to build buffer muon cut parameters.");
    
    return input;
  
  }
  
}

#endif