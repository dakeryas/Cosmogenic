#ifndef COSMOGENIC_MUON_H
#define COSMOGENIC_MUON_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/Segment.hpp"

namespace CosmogenicHunter{

  template <class T>
  class MuonDefinition;
  
  template <class T>
  class Single;
  
  template <class T>
  class Muon : public Event<T>{
    
    Segment<T> track;
    T vetoCharge;
    T detectorCharge;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Muon() = default;
    Muon(double triggerTime, T visibleEnergy, unsigned identifier, Segment<T> track, T vetoCharge, T detectorCharge);
    const Segment<T>& getTrack() const;
    T getVetoCharge() const;
    T getDetectorCharge() const;
    T getVisibleEnergy(const MuonDefinition<T>& muonDefinition) const;
    T getDistanceTo(const Single<T>& single) const;//shortest distance between track and single's position
    bool triggersInnerVeto(T maxInnerVetoCharge) const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Muon<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), track, vetoCharge, detectorCharge);

  }
  
  template <class T>
  Muon<T>::Muon(double triggerTime, T visibleEnergy, unsigned identifier, Segment<T> track, T vetoCharge, T detectorCharge)
  :Event<T>(triggerTime, visibleEnergy, identifier),track(std::move(track)),vetoCharge(vetoCharge),detectorCharge(detectorCharge){
    
  }
  
  template <class T>
  const Segment<T>& Muon<T>::getTrack() const{
    
    return track;

  }
  
  template<class T>
  T Muon<T>::getVetoCharge() const{
    
    return vetoCharge;

  }
  
  template <class T>
  T Muon<T>::getDetectorCharge() const{
    
    return detectorCharge;

  }
  
  template <class T>
  T Muon<T>::getVisibleEnergy(const MuonDefinition<T>& muonDefinition) const{
    
    muonDefinition.getVisibleEnergy(*this);

  }
  
  template <class T>
  T Muon<T>::getDistanceTo(const Single<T>& single) const{

    return single.getDistanceTo(*this);
  
  }
  
  template<class T>
  bool Muon<T>::triggersInnerVeto(T maxInnerVetoCharge) const{
    
    return vetoCharge > maxInnerVetoCharge;

  }
  
  template <class T>
  void Muon<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    unsigned firstColumnWidth = 13;
    Event<T>::print(output, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Track"<<": "<<track
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"IV charge"<<": "<<std::setw(6)<<std::left<<vetoCharge
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"ID Charge"<<": "<<std::setw(6)<<std::left<<detectorCharge;

  }

  template <class T>
  std::ostream& operator<<(std::ostream& output, const Muon<T>& muon){
    
    muon.print(output, 0);
    return output;
    
  }

}

#endif