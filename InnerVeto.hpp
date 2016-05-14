#ifndef COSMOGENIC_INNER_VETO_THRESHOLD_H
#define COSMOGENIC_INNER_VETO_THRESHOLD_H

#include "Cosmogenic/Veto.hpp"
#include "Cosmogenic/Bounds.hpp"
#include "Cosmogenic/Point.hpp"

namespace CosmogenicHunter{
  
  template <class T>
  class InnerVeto : public Veto<T>{
    
    T maxCharge;
    int maxNumberOfHitPMTs;
    Bounds<T> timeCorrelationBounds;//time correlation to the inner detector start time
    T minDistanceToInnerDetector;
    
    bool veto(const InnerVetoInformation<T>& innerVetoInformation) const;
    
  public:
    InnerVeto();
    InnerVeto(T maxCharge, int maxNumberOfHitPMTs, Bounds<T> timeCorrelationBounds, T minDistanceToInnerDetector);
    T getMaxCharge() const;
    int getMaxNumberOfHitPMTs() const;
    const Bounds<T>& getTimeCorrelationBounds() const;
    T getMinDistanceToInnerDetector() const;
    void setMaxCharge(T maxCharge);
    void setMaxNumberOfHitPMTs(int maxNumberOfHitPMTs);
    void setTimeCorrelationBounds(Bounds<T> timeCorrelationBounds);
    void setMinDistanceToInnerDetector(T minDistanceToInnerDetector);
    bool veto(const Single<T>& single) const;
    bool veto(const CandidatePair<T>& candidatePair) const;
    std::unique_ptr<Veto<T>> clone() const;
    void print(std::ostream& output) const;
    
  };
  
  template <class T>
  InnerVeto<T>::InnerVeto()
  :InnerVeto<T>(std::numeric_limits<T>::max(), std::numeric_limits<unsigned short>::max(), Bounds<T>(0, 0), 0){
    
  }
  
  template <class T>
  InnerVeto<T>::InnerVeto(T maxCharge, int maxNumberOfHitPMTs, Bounds<T> timeCorrelationBounds, T minDistanceToInnerDetector)
  :Veto<T>("InnerVeto"),maxCharge(maxCharge),maxNumberOfHitPMTs(maxNumberOfHitPMTs),timeCorrelationBounds(std::move(timeCorrelationBounds)),minDistanceToInnerDetector(minDistanceToInnerDetector){
    
    if(maxCharge < 0 || maxNumberOfHitPMTs < 0 ||  maxNumberOfHitPMTs > std::numeric_limits<unsigned short>::max() || minDistanceToInnerDetector < 0){
      
      auto errorMessage = std::to_string(maxCharge)+"DUQ, "+std::to_string(maxNumberOfHitPMTs)+", and "+std::to_string(minDistanceToInnerDetector)+"mm are invalid inner veto charge, number of hit PMTs, and distance to inner detector.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T InnerVeto<T>::getMaxCharge() const{
    
    return maxCharge;

  }

  template <class T>
  int InnerVeto<T>::getMaxNumberOfHitPMTs() const{
    
    return maxNumberOfHitPMTs;

  }
  
  template <class T>
  const Bounds<T>& InnerVeto<T>::getTimeCorrelationBounds() const{
    
    return timeCorrelationBounds;

  }

  template <class T>
  T InnerVeto<T>::getMinDistanceToInnerDetector() const{
    
    return minDistanceToInnerDetector;

  }
  
  template <class T>
  void InnerVeto<T>::setMaxCharge(T maxCharge){
    
    if(maxCharge >= 0) this->maxCharge = maxCharge;
    else throw std::invalid_argument(std::to_string(maxCharge)+"DUQ is not a valid inner veto charge.");

  }
  
  template <class T>
  void InnerVeto<T>::setMaxNumberOfHitPMTs(int maxNumberOfHitPMTs){
    
    if(maxNumberOfHitPMTs >= 0 && maxNumberOfHitPMTs <= std::numeric_limits<unsigned short>::max()) this->maxNumberOfHitPMTs = maxNumberOfHitPMTs;
    else throw std::invalid_argument(std::to_string(maxNumberOfHitPMTs)+" is not a valid number of hit PMTs.");

  }
  
  template <class T>
  void InnerVeto<T>::setTimeCorrelationBounds(Bounds<T> timeCorrelationBounds){
    
    this->timeCorrelationBounds = timeCorrelationBounds;

  }
  
  template <class T>
  void InnerVeto<T>::setMinDistanceToInnerDetector(T minDistanceToInnerDetector){
    
    if(minDistanceToInnerDetector >= 0) this->minDistanceToInnerDetector = minDistanceToInnerDetector;
    else throw std::invalid_argument(std::to_string(maxCharge)+"mm is not a valid distance to inner detector.");

  }
  
  template <class T>
  bool InnerVeto<T>::veto(const InnerVetoInformation<T>& innerVetoInformation) const{

    return innerVetoInformation.getCharge() > maxCharge && innerVetoInformation.getNumberOfHitPMTs() >= maxNumberOfHitPMTs && timeCorrelationBounds.contains(innerVetoInformation.getTimeToInnerDetectorStart()) && innerVetoInformation.getDistanceToInnerDetector() < minDistanceToInnerDetector;

  }
  
  template <class T>
  bool InnerVeto<T>::veto(const Single<T>& single) const{
    
    return veto(single.getInnerVetoInformation());
    
  }
  
  template <class T>
  bool InnerVeto<T>::veto(const CandidatePair<T>& candidatePair) const{

    return veto(candidatePair.getPrompt());

  }
  
  template <class T>
  std::unique_ptr<Veto<T>> InnerVeto<T>::clone() const{

    return std::make_unique<InnerVeto<T>>(*this);

  }
  
  template <class T>
  void InnerVeto<T>::print(std::ostream& output) const{
    
    int labelColumnWidth = 12;
    int dataColumnWidth = 6;
    
    output<<std::setw(labelColumnWidth)<<std::left<<"Max charge"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxCharge<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Max Hit PMTs"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxNumberOfHitPMTs<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Time bounds"<<": "<<timeCorrelationBounds<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Min distance"<<": "<<minDistanceToInnerDetector;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVeto<T>& innerVeto){
    
    innerVeto.print(output);
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, InnerVeto<T>& innerVeto){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::string regexString = "^(";
    for(unsigned k = 0; k < 4; ++k) regexString += number+")[:,](";//start with a number :, seprator
    regexString += number+")$";
    
    std::regex regex(regexString);
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex))
      innerVeto = std::move(InnerVeto<T>(std::stod(regexMatches[1]), std::stod(regexMatches[2]), Bounds<T>(std::stod(regexMatches[3]), std::stod(regexMatches[4])), std::stod(regexMatches[5])));
    else throw std::invalid_argument(token+" cannot be parsed to build inner veto threshold.");
    
    return input;
  
  }
  
}

#endif