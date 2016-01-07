#ifndef COSMOGENIC_SINGLE_H
#define COSMOGENIC_SINGLE_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/Point.hpp"
#include "Cosmogenic/ChargeInformation.hpp"

namespace CosmogenicHunter{

  template <class T>
  class LightNoiseCutParameters;
  
  template <class T>
  class Single : public Event<T>{
    
    Point<T> position;//RecoBAMA reconstructed positon
    T reconstructionGoodness;//RecoBAMA global fitting functional value
    ChargeInformation<T> chargeInformation;//QRMS, QDiff, QRatio, startTimeRMS
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Single();
    Single(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position, T reconstructionGoodness, ChargeInformation<T> chargeInformation);
    const Point<T>& getPosition() const;
    T getReconstructionGoodness() const;
    const ChargeInformation<T>& getChargeInformation() const;
    double getSpaceCorrelation(const Single<T>& other) const;
    bool isSpaceCorrelated(const Single<T>& other, double maxDistance) const;
    bool isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Single<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), position, reconstructionGoodness, chargeInformation);

  }
  
  template <class T>
  Single<T>::Single():reconstructionGoodness(0){
    
  }
  
  template <class T>
  Single<T>::Single(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position, T reconstructionGoodness, ChargeInformation<T> chargeInformation)
  :Event<T>(triggerTime, vetoCharge, visibleEnergy, identifier), position(std::move(position)),reconstructionGoodness(reconstructionGoodness),chargeInformation(std::move(chargeInformation)){
    
  }

  template <class T>
  const Point<T>& Single<T>::getPosition() const{
    
    return position;

  }
  
  template <class T>
  T Single<T>::getReconstructionGoodness() const{
    
    return reconstructionGoodness;
    
  }
  
  template <class T>
  const ChargeInformation<T>& Single<T>::getChargeInformation() const{
    
    return chargeInformation;
    
  }

  template <class T>
  double Single<T>::getSpaceCorrelation(const Single<T>& other) const{

    return getDistanceBetween(position, other.position);
  
  }

  template <class T>
  bool Single<T>::isSpaceCorrelated(const Single<T>& other, double maxDistance) const{

    return getSpaceCorrelation(other) < maxDistance;
  
  }
  
  template <class T>
  bool Single<T>::isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const{
    
    return lightNoiseCutParameters.accept(chargeInformation);
    
  }
  
  template <class T>
  void Single<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    Event<T>::print(output, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Position"<<": "<<position
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Goodness"<<": "<<reconstructionGoodness
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Charge"<<":\n";
    chargeInformation.print(output, outputOffset + 3);  

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const Single<T>& single){
    
    single.print(output, 0);
    return output;
    
  }

  template <class T>
  double getSpaceCorrelation(const Single<T>& single1, const Single<T>& single2){

    return single1.getSpaceCorrelation(single2);
  
  }

  template <class T>
  bool areSpaceCorrelated(const Single<T>& single1, const Single<T>& single2, double maxDistance){

    return single1.isSpaceCorrelated(single2, maxDistance);
  
  }

}


#endif