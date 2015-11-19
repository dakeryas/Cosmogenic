#ifndef MUON_H
#define MUON_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/Segment.hpp"

namespace CosmogenicHunter{

  template <class T>
  class Muon : public Event<T>{
    
    Segment<T> track;
    T detectorCharge;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Muon() = default;
    Muon(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Segment<T> track, T detectorCharge);
    const Segment<T>& getTrack() const;
    T getDetectorCharge() const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Muon<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), track, detectorCharge);

  }
  
  template <class T>
  Muon<T>::Muon(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Segment<T> track, T detectorCharge)
  :Event<T>(triggerTime, vetoCharge, visibleEnergy, identifier), track(std::move(track)), detectorCharge(detectorCharge){
    
  }
  
  template <class T>
  const Segment<T>& Muon<T>::getTrack() const{
    
    return track;

  }
  
  template <class T>
  T Muon<T>::getDetectorCharge() const{
    
    return detectorCharge;

  }
  
  template <class T>
  void Muon<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    Event<T>::print(output, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Track"<<": "<<track
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"ID Charge"<<": "<<std::setw(6)<<std::left<<detectorCharge;

  }

  template <class T>
  std::ostream& operator<<(std::ostream& output, const Muon<T>& muon){
    
    muon.print(output, 0);
    return output;
    
  }

}

#endif