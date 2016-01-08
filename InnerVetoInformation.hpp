#ifndef COSMOGENIC_INNER_VETO_INFORMATION_H
#define COSMOGENIC_INNER_VETO_INFORMATION_H

#include <iomanip>
#include "cereal/archives/binary.hpp"

namespace CosmogenicHunter{

  template <class T>
  class InnerVetoInformation{
    
    T charge;
    unsigned short numberOfHitPMTs;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    InnerVetoInformation();
    InnerVetoInformation(T charge, unsigned short numberOfHitPMTs);
    T getCharge() const;
    unsigned short getNumberOfHitPMTs() const;
    void setCharge(T charge);
    void setNumberOfHitPMTs(unsigned short numberOfHitPMTs);
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void InnerVetoInformation<T>::serialize(Archive& archive){
    
    archive(charge, numberOfHitPMTs);

  }
  
  template <class T>
  InnerVetoInformation<T>::InnerVetoInformation():charge(0),numberOfHitPMTs(0){
    
  }
  
  template <class T>
  InnerVetoInformation<T>::InnerVetoInformation(T charge, unsigned short numberOfHitPMTs)
  :charge(charge),numberOfHitPMTs(numberOfHitPMTs){
    
     if(charge < 0) throw std::invalid_argument(std::to_string(charge)+"DUQ is not a valid inner veto charge.");
    
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
  void InnerVetoInformation<T>::setCharge(T charge){
    
    if(charge >= 0) this->charge = charge;
    else throw std::invalid_argument(std::to_string(charge)+"DUQ is not a valid inner veto charge.");

  }
  
  template <class T>
  void InnerVetoInformation<T>::setNumberOfHitPMTs(unsigned short numberOfHitPMTs){
    
    this->numberOfHitPMTs = numberOfHitPMTs;

  }
  
  template <class T>
  void InnerVetoInformation<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    output<<std::setw(outputOffset)<<std::left<<" "<<std::setw(13)<<std::left<<"Charge"<<": "<<std::setw(7)<<std::left<<charge<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(13)<<std::left<<"Hit PMTs"<<": "<<std::setw(4)<<std::left<<numberOfHitPMTs;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVetoInformation<T>& innerVetoInformation){
    
    innerVetoInformation.print(output, 0);
    return output;
    
  }

}

#endif