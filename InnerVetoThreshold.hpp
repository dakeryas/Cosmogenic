#ifndef COSMOGENIC_INNER_VETO_THRESHOLD_H
#define COSMOGENIC_INNER_VETO_THRESHOLD_H

#include "Cosmogenic/Bounds.hpp"
#include "Cosmogenic/Point.hpp"

namespace CosmogenicHunter{
  
  template <class T>
  class InnerVetoThreshold{
    
    T maxCharge;
    int maxNumberOfHitPMTs;
    Bounds<T> timeCorrelationBounds;//time correlation to the inner detector start time
    T maxDistanceToInnerDetector;
    
  public:
    InnerVetoThreshold() = default;
    InnerVetoThreshold(T maxCharge, int maxNumberOfHitPMTs, Bounds<T> timeCorrelationBounds, T maxDistanceToInnerDetector);
    T getMaxCharge() const;
    int getMaxNumberOfHitPMTs() const;
    const Bounds<T>& getTimeCorrelationBounds() const;
    T getMaxDistanceToInnerDetector() const;
    void setMaxCharge(T maxCharge);
    void setMaxNumberOfHitPMTs(int maxNumberOfHitPMTs);
    void setTimeCorrelationBounds(Bounds<T> timeCorrelationBounds);
    void setMaxDistanceToInnerDetector(T maxDistanceToInnerDetector);
    bool tag(const InnerVetoInformation<T>& innerVetoInformation) const;
    bool tag(const Single<T>& single) const;
    
  };
  
  template <class T>
  InnerVetoThreshold<T>::InnerVetoThreshold(T maxCharge, int maxNumberOfHitPMTs, Bounds<T> timeCorrelationBounds, T maxDistanceToInnerDetector)
  :maxCharge(maxCharge),maxNumberOfHitPMTs(maxNumberOfHitPMTs),timeCorrelationBounds(std::move(timeCorrelationBounds)),maxDistanceToInnerDetector(maxDistanceToInnerDetector){
    
    if(maxCharge < 0 || maxNumberOfHitPMTs < 0 ||  maxNumberOfHitPMTs > std::numeric_limits<unsigned short>::max() || maxDistanceToInnerDetector < 0){
      
      auto errorMessage = std::to_string(maxCharge)+"DUQ, "+std::to_string(maxNumberOfHitPMTs)+", and "+std::to_string(maxDistanceToInnerDetector)+"mm are invalid inner veto charge, number of hit PMTs, and distance to inner detector.";
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
  const Bounds<T>& InnerVetoThreshold<T>::getTimeCorrelationBounds() const{
    
    return timeCorrelationBounds;

  }

  template <class T>
  T InnerVetoThreshold<T>::getMaxDistanceToInnerDetector() const{
    
    return maxDistanceToInnerDetector;

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
  void InnerVetoThreshold<T>::setTimeCorrelationBounds(Bounds<T> timeCorrelationBounds){
    
    this->timeCorrelationBounds = timeCorrelationBounds;

  }
  
  template <class T>
  void InnerVetoThreshold<T>::setMaxDistanceToInnerDetector(T maxDistanceToInnerDetector){
    
    if(maxDistanceToInnerDetector >= 0) this->maxDistanceToInnerDetector = maxDistanceToInnerDetector;
    else throw std::invalid_argument(std::to_string(maxCharge)+"mm is not a valid distance to inner detector.");

  }
  
  template <class T>
  bool InnerVetoThreshold<T>::tag(const InnerVetoInformation<T>& innerVetoInformation) const{

    return innerVetoInformation.getCharge() > maxCharge && innerVetoInformation.getNumberOfHitPMTs() >= maxNumberOfHitPMTs && timeCorrelationBounds.contains(innerVetoInformation.getTimeToInnerDetectorStart()) && innerVetoInformation.getDistanceToInnerDetector() < maxDistanceToInnerDetector;

  }
  
  template <class T>
  bool InnerVetoThreshold<T>::tag(const Single<T>& single) const{
    
    return tag(single.getInnerVetoInformation());
    
  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVetoThreshold<T>& innerVetoThreshold){

    output<<std::setw(12)<<std::left<<"Max charge"<<": "<<std::setw(6)<<std::right<<innerVetoThreshold.getMaxCharge()<<"\n"
      <<std::setw(12)<<std::left<<"Max Hit PMTs"<<": "<<std::setw(6)<<std::right<<innerVetoThreshold.getMaxNumberOfHitPMTs()<<"\n"
      <<std::setw(12)<<std::left<<"Time bounds"<<": "<<innerVetoThreshold.getTimeCorrelationBounds()<<"\n"
      <<std::setw(12)<<std::left<<"Max distance"<<": "<<innerVetoThreshold.getMaxDistanceToInnerDetector();
      
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, InnerVetoThreshold<T>& innerVetoThreshold){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::string regexString = "^(";
    for(unsigned k = 0; k < 4; ++k) regexString += number+")[:,](";//start with a number :, seprator
    regexString += number+")$";
    
    std::regex regex(regexString);
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex))
      innerVetoThreshold = std::move(InnerVetoThreshold<T>(std::stod(regexMatches[1]), std::stod(regexMatches[2]), Bounds<T>(std::stod(regexMatches[3]), std::stod(regexMatches[4])), std::stod(regexMatches[5])));
    else throw std::invalid_argument(token+" cannot be parsed to build inner veto threshold.");
    
    return input;
  
  }
  
}

#endif