#ifndef COSMOGENIC_BUFFER_MUON_CUT_PARAMETERS_H
#define COSMOGENIC_BUFFER_MUON_CUT_PARAMETERS_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{
  
  template <class T>
  class BufferMuonCutParameters{
    
    T constant;
    T exponent;
    
  public:
    BufferMuonCutParameters() = default;
    BufferMuonCutParameters(T constant, T exponent);
    T getConstant() const;
    T getExponent() const;
    void setConstant(T constant);
    void setExponent(T exponent);
    void setParameters(T constant, T exponent);
    bool tag(const Single<T>& single) const;
    
  };
  
  template <class T>
  BufferMuonCutParameters<T>::BufferMuonCutParameters(T constant, T exponent)
  :constant(constant),exponent(exponent){
    
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
  bool BufferMuonCutParameters<T>::tag(const Single<T>& single) const{

    return single.getChargeInformation().getRatio() > constant / std::pow(single.getVisibleEnergy(), exponent); // the charge ratio is too high for such a small energy

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const BufferMuonCutParameters<T>& bufferMuonCutParameters){

    output<<std::setw(8)<<std::left<<"Constant"<<": "<<std::setw(6)<<std::right<<bufferMuonCutParameters.getConstant()<<"\n"
      <<std::setw(8)<<std::left<<"Exponent"<<": "<<std::setw(6)<<std::right<<bufferMuonCutParameters.getExponent();
      
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