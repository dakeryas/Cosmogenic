#ifndef COSMOGENIC_INNER_VETO_THRESHOLD_H
#define COSMOGENIC_INNER_VETO_THRESHOLD_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{
  
  template <class T>
  class InnerVetoThreshold{
    
    T maxCharge;
    int maxNumberOfHitPMTs;
    
  public:
    InnerVetoThreshold() = default;
    InnerVetoThreshold(T maxCharge, int maxNumberOfHitPMTs);
    T getMaxCharge() const;
    int getMaxNumberOfHitPMTs() const;
    void setMaxCharge(T maxCharge);
    void setMaxNumberOfHitPMTs(int maxNumberOfHitPMTs);
    void setParameters(T maxCharge, int maxNumberOfHitPMTs);
    bool tag(const InnerVetoData<T>& innerVetoData) const;
    bool tag(const InnerVetoInformation<T>& innerVetoInformation) const;
    
  };
  
  template <class T>
  InnerVetoThreshold<T>::InnerVetoThreshold(T maxCharge, int maxNumberOfHitPMTs)
  :maxCharge(maxCharge),maxNumberOfHitPMTs(maxNumberOfHitPMTs){
    
    if(maxCharge < 0 || maxNumberOfHitPMTs < 0 ||  maxNumberOfHitPMTs > std::numeric_limits<unsigned short>::max()){
      
      auto errorMessage = std::to_string(maxCharge)+"DUQ and "+std::to_string(maxNumberOfHitPMTs)+" are not a valid inner veto charge and number of hit PMTs.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T InnerVetoThreshold<T>::getMaxCharge() const{
    
    return maxCharge;

  }

  template <class T>
  int InnerVetoThreshold<T>::getMaxNumberOfHitPMTs() const{
    
    return maxNumberOfHitPMTs;

  }
  
  template <class T>
  void InnerVetoThreshold<T>::setMaxCharge(T maxCharge){
    
    if(maxCharge >= 0) this->maxCharge = maxCharge;
    else throw std::invalid_argument(std::to_string(maxCharge)+"DUQ is not a valid inner veto charge.");

  }
  
  template <class T>
  void InnerVetoThreshold<T>::setMaxNumberOfHitPMTs(int maxNumberOfHitPMTs){
    
    if(maxNumberOfHitPMTs >= 0 && maxNumberOfHitPMTs <= std::numeric_limits<unsigned short>::max()) this->maxNumberOfHitPMTs = maxNumberOfHitPMTs;
    else throw std::invalid_argument(std::to_string(maxNumberOfHitPMTs)+" is not a valid number of hit PMTs.");

  }
  
  template <class T>
  void InnerVetoThreshold<T>::setParameters(T maxCharge, int maxNumberOfHitPMTs){

    setMaxCharge(maxCharge);
    setMaxNumberOfHitPMTs(maxNumberOfHitPMTs);

  }
  
  template <class T>
  bool InnerVetoThreshold<T>::tag(const InnerVetoData<T>& innerVetoData) const{
    
    return innerVetoData.charge > maxCharge && innerVetoData.numberOfHitPMTs >= maxNumberOfHitPMTs;

  }
  
  template <class T>
  bool InnerVetoThreshold<T>::tag(const InnerVetoInformation<T>& innerVetoInformation) const{

    return innerVetoInformation.getCharge() > maxCharge && innerVetoInformation.getNumberOfHitPMTs() >= maxNumberOfHitPMTs;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVetoThreshold<T>& innerVetoThreshold){

    output<<std::setw(12)<<std::left<<"Max charge"<<": "<<std::setw(6)<<std::right<<innerVetoThreshold.getMaxCharge()<<"\n"
      <<std::setw(12)<<std::left<<"Max Hit PMTs"<<": "<<std::setw(6)<<std::right<<innerVetoThreshold.getMaxNumberOfHitPMTs();
      
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, InnerVetoThreshold<T>& innerVetoThreshold){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("(^"+number+")[:,]("+number+"$)");//start with a number :, seprator and end with another number
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) innerVetoThreshold.setParameters(std::stod(regexMatches[1]), std::stod(regexMatches[2]));
    else throw std::invalid_argument(token+" cannot be parsed to build inner veto threshold.");
    
    return input;
  
  }
  
}

#endif