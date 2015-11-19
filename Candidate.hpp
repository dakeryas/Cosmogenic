#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/Point.hpp"
#include "Cosmogenic/ChargeInformation.hpp"

namespace CosmogenicHunter{

  template <class T>
  class Candidate : public Event<T>{
    
    Point<T> position;//RecoBAMA reconstructed positon
    T reconstructionGoodness;//RecoBAMA global fitting functional value
    ChargeInformation<T> chargeInformation;//QRMS, QDiff, QRatio, startTimeRMS
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Candidate() = default;
    Candidate(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position, T reconstructionGoodness, ChargeInformation<T> chargeInformation);
    const Point<T>& getPosition() const;
    T getReconstructionGoodness() const;
    const ChargeInformation<T>& getChargeInformation() const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Candidate<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), position, reconstructionGoodness, chargeInformation);

  }
  
  template <class T>
  Candidate<T>::Candidate(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position, T reconstructionGoodness, ChargeInformation<T> chargeInformation)
  :Event<T>(triggerTime, vetoCharge, visibleEnergy, identifier), position(std::move(position)),reconstructionGoodness(reconstructionGoodness),chargeInformation(std::move(chargeInformation)){
    
  }

  template <class T>
  const Point<T>& Candidate<T>::getPosition() const{
    
    return position;

  }
  
  template <class T>
  T Candidate<T>::getReconstructionGoodness() const{
    
    return reconstructionGoodness;
    
  }
  
  template <class T>
  const ChargeInformation<T>& Candidate<T>::getChargeInformation() const{
    
    return chargeInformation;
    
  }
  
  template <class T>
  void Candidate<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    Event<T>::print(output, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Position"<<": "<<position
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Goodness"<<": "<<reconstructionGoodness
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Charge"<<":\n";
    chargeInformation.print(output, outputOffset + 3);  

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const Candidate<T>& candidate){
    
    candidate.print(output, 0);
    return output;
    
  }

}


#endif