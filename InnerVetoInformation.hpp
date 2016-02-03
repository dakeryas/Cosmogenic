#ifndef COSMOGENIC_INNER_VETO_INFORMATION_H
#define COSMOGENIC_INNER_VETO_INFORMATION_H

#include <iomanip>
#include "cereal/archives/binary.hpp"

namespace CosmogenicHunter{

  template <class T>
  class InnerVetoInformation{
    
    T charge;
    unsigned short numberOfHitPMTs;
    T timeToInnerDetectorStart;
    T distanceToInnerDetector;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    InnerVetoInformation();
    InnerVetoInformation(T charge, unsigned short numberOfHitPMTs, T timeToInnerDetectorStart, T distanceToInnerDetector);
    T getCharge() const;
    unsigned short getNumberOfHitPMTs() const;
    T getTimeToInnerDetectorStart() const;
    T getDistanceToInnerDetector() const;
    void setCharge(T charge);
    void setNumberOfHitPMTs(unsigned short numberOfHitPMTs);
    void setTimeToInnerDetectorStart(T timeToInnerDetectorStart);
    void setDistanceToInnerDetector(T distanceToInnerDetector);
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void InnerVetoInformation<T>::serialize(Archive& archive){
    
    archive(charge, numberOfHitPMTs, timeToInnerDetectorStart, distanceToInnerDetector);

  }
  
  template <class T>
  InnerVetoInformation<T>::InnerVetoInformation():charge(0),numberOfHitPMTs(0){
    
  }
  
  template <class T>
  InnerVetoInformation<T>::InnerVetoInformation(T charge, unsigned short numberOfHitPMTs, T timeToInnerDetectorStart, T distanceToInnerDetector)
  :charge(charge),numberOfHitPMTs(numberOfHitPMTs),timeToInnerDetectorStart(timeToInnerDetectorStart),distanceToInnerDetector(distanceToInnerDetector){
    
    if(charge < 0 || distanceToInnerDetector < 0){
      
      auto errorMessage = std::to_string(charge)+"DUQ and "+std::to_string(distanceToInnerDetector)+"mm are invalid inner veto charge and distance to inner detector.";
      throw std::invalid_argument(errorMessage);
    }
    
  }

  template <class T>
  T InnerVetoInformation<T>::getCharge() const{
    
    return charge;

  }

  template <class T>
  unsigned short InnerVetoInformation<T>::getNumberOfHitPMTs() const{
    
    return numberOfHitPMTs;

  }
  
  template <class T>
  T InnerVetoInformation<T>::getTimeToInnerDetectorStart() const{
    
    return timeToInnerDetectorStart;

  }
  
  template <class T>
  T InnerVetoInformation<T>::getDistanceToInnerDetector() const{
    
    return distanceToInnerDetector;

  }
  
  template <class T>
  void InnerVetoInformation<T>::setCharge(T charge){
    
    if(charge >= 0) this->charge = charge;
    else throw std::invalid_argument(std::to_string(charge)+"DUQ is not a valid inner veto charge.");

  }
  
  template <class T>
  void InnerVetoInformation<T>::setNumberOfHitPMTs(unsigned short numberOfHitPMTs){
    
    this->numberOfHitPMTs = numberOfHitPMTs;

  }
  
  template <class T>
  void InnerVetoInformation<T>::setTimeToInnerDetectorStart(T timeToInnerDetectorStart){
    
    this->timeToInnerDetectorStart = timeToInnerDetectorStart;

  }
  
  template <class T>
  void InnerVetoInformation<T>::setDistanceToInnerDetector(T distanceToInnerDetector){
    
    if(distanceToInnerDetector >= 0) this->distanceToInnerDetector = distanceToInnerDetector;
    else throw std::invalid_argument(std::to_string(distanceToInnerDetector)+"mm is not a valid distance to inner detector.");

  }
  
  template <class T>
  void InnerVetoInformation<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    unsigned firstColumnWidth = 14;
    output<<std::setw(outputOffset)<<std::left<<" "<<std::setw(firstColumnWidth)<<std::left<<"Charge"<<": "<<std::setw(7)<<std::left<<charge<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(firstColumnWidth)<<std::left<<"Hit PMTs"<<": "<<std::setw(4)<<std::left<<numberOfHitPMTs<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(firstColumnWidth)<<std::left<<"Time to ID"<<": "<<std::setw(4)<<std::left<<timeToInnerDetectorStart<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(firstColumnWidth)<<std::left<<"Distance to ID"<<": "<<std::setw(5)<<std::left<<distanceToInnerDetector;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVetoInformation<T>& innerVetoInformation){
    
    innerVetoInformation.print(output, 0);
    return output;
    
  }

}

#endif