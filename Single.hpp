#ifndef COSMOGENIC_SINGLE_H
#define COSMOGENIC_SINGLE_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/PositionInformation.hpp"
#include "Cosmogenic/InnerVetoInformation.hpp"
#include "Cosmogenic/ChargeInformation.hpp"

namespace CosmogenicHunter{

  template <class T>
  class Muon;
  
  template <class T>
  class LightNoiseCutParameters;
  
  template <class T>
  class InnerVetoThreshold;
  
  template <class T>
  class ReconstructionCutParameters;
  
  template <class T>
  class Single : public Event<T>{
    
    PositionInformation<T> positionInformation;//RecoBAMA reconstructed positon and functional value
    InnerVetoInformation<T> innerVetoInformation;//chargeIV, number of hit IV PMTs
    ChargeInformation<T> chargeInformation;//QRMS, QDiff, QRatio, startTimeRMS
    T chimneyInconsistencyRatio;///ratio: minus log (pulse shape likelihood in the chimney) / minus log (pulse shape likelihood at reconstruction positon)
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Single();
    Single(double triggerTime, T visibleEnergy, unsigned identifier, PositionInformation<T> positionInformation, InnerVetoInformation<T> innerVetoInformation, ChargeInformation<T> chargeInformation, T chimneyInconsistencyRatio);
    const PositionInformation<T>& getPositionInformation() const;
    const InnerVetoInformation<T>& getInnerVetoInformation() const;
    const ChargeInformation<T>& getChargeInformation() const;
    T getChimneyInconsistencyRatio() const;
    T getDistanceTo(const Muon<T>& muon) const;//shortest distance to Muon's track
    T getSpaceCorrelation(const Single<T>& other) const;
    bool isSpaceCorrelated(const Single<T>& other, double maxDistance) const;
    bool isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const;
    bool isVetoed(const InnerVetoThreshold<T>& innerVetoThreshold) const;
     bool isPoorlyReconstructed(const ReconstructionCutParameters<T>& reconstructionCutParameters) const;
    void print(std::ostream& output, unsigned firstColumnWidth, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Single<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), positionInformation, innerVetoInformation, chargeInformation, chimneyInconsistencyRatio);

  }
  
  template <class T>
  Single<T>::Single():chimneyInconsistencyRatio(std::numeric_limits<T>::max()){
    
  }

  template <class T>
  Single<T>::Single(double triggerTime, T visibleEnergy, unsigned identifier, PositionInformation<T> positionInformation, InnerVetoInformation<T> innerVetoInformation, ChargeInformation<T> chargeInformation, T chimneyInconsistencyRatio)
  :Event<T>(triggerTime, visibleEnergy, identifier),positionInformation(std::move(positionInformation)),innerVetoInformation(std::move(innerVetoInformation)),chargeInformation(std::move(chargeInformation)),chimneyInconsistencyRatio(chimneyInconsistencyRatio){
    
  }
  
  template <class T>
  const PositionInformation<T>& Single<T>::getPositionInformation() const{
    
    return positionInformation;

  }

  template <class T>
  const InnerVetoInformation<T>& Single<T>::getInnerVetoInformation() const{
    
    return innerVetoInformation;

  }
  
  template <class T>
  const ChargeInformation<T>& Single<T>::getChargeInformation() const{
    
    return chargeInformation;
    
  }
  
  template <class T>
  T Single<T>::getChimneyInconsistencyRatio() const{
    
    return chimneyInconsistencyRatio;
    
  }
  
  template <class T>
  T Single<T>::getDistanceTo(const Muon<T>& muon) const{

    return getDistanceBetween(positionInformation.getPosition(), muon.getTrack());
  
  }

  template <class T>
  T Single<T>::getSpaceCorrelation(const Single<T>& other) const{

    return getDistanceBetween(positionInformation.getPosition(), other.getPositionInformation().getPosition());
  
  }

  template <class T>
  bool Single<T>::isSpaceCorrelated(const Single<T>& other, double maxDistance) const{

    return getSpaceCorrelation(other) < maxDistance;
  
  }
  
  template <class T>
  bool Single<T>::isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const{
    
    return lightNoiseCutParameters.tag(chargeInformation);
    
  }
  
  template <class T>
  bool Single<T>::isVetoed(const InnerVetoThreshold<T>& innerVetoThreshold) const{
    
    return innerVetoThreshold.tag(innerVetoInformation);
    
  }
  
  template <class T>
  bool Single<T>::isPoorlyReconstructed(const ReconstructionCutParameters<T>& reconstructionCutParameters) const{

    return reconstructionCutParameters.tag(*this);
  
  }
  
  template <class T>
  void Single<T>::print(std::ostream& output, unsigned firstColumnWidth, unsigned outputOffset) const{
    
    Event<T>::print(output, firstColumnWidth, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Position"<<":\n";
    positionInformation.print(output, outputOffset + 3);
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Inner Veto"<<":\n";
    innerVetoInformation.print(output, outputOffset + 3);
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Charge"<<":\n";
    chargeInformation.print(output, outputOffset + 3);
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Chimney Pulse"<<":\n"
    <<std::setw(outputOffset + 3)<<std::left<<""<<"Inconsistency: "<<chimneyInconsistencyRatio;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const Single<T>& single){
    
    single.print(output, 13, 0);
    return output;
    
  }
  
  template <class T>
  T getDistanceBetween(const Single<T>& single, const Muon<T>& muon){
    
    return single.getDistanceTo(muon);
    
  }
  
  template <class T>
  T getDistanceBetween(const Muon<T>& muon, const Single<T>& single){
    
    return getDistanceBetween(single, muon);
    
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