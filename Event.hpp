#ifndef COSMOGENIC_EVENT_H
#define COSMOGENIC_EVENT_H

#include <iomanip>
#include "cereal/archives/binary.hpp"
#include "Cosmogenic/Bounds.hpp"

namespace CosmogenicHunter{

  template <class T>
  class Event{
  
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  protected:
    double triggerTime;
    T visibleEnergy;
    unsigned identifier;

  public:
    Event();
    Event(double triggerTime, T visibleEnergy, unsigned identifier);
    Event(const Event<T>& other) = default;
    Event(Event&& other) = default;
    Event& operator = (const Event<T>& other) = default;
    Event& operator = (Event&& other) = default;
    virtual ~Event() = default;//custom destructor implies to define (even if default-ed) all copy / move / assignement operations
    double getTriggerTime() const;
    T getVisibleEnergy() const;
    unsigned getIdentifier() const;//identifier of the event within the corresponding run
    double getTimeCorrelation(const Event<T>& other) const;
    bool isTimeCorrelated(const Event<T>& other, const Bounds<double>& timeBounds) const;
    bool hasVisibleEnergyWithin(const Bounds<T>& energyBounds) const;
    virtual void print(std::ostream& output, unsigned outputOffset) const;//needed to act as if 'operator<<' was virtual
    bool isEqualTo(const Event<T>& other) const;//checks identifiers only
    
  };
  
  template<class T>
  template <class Archive>
  void Event<T>::serialize(Archive& archive){
    
    archive(triggerTime, visibleEnergy, identifier);

  }
  
  template<class T>
  Event<T>::Event():triggerTime(0),visibleEnergy(0),identifier(0){
    
  }

  template<class T>
  Event<T>::Event(double triggerTime, T visibleEnergy, unsigned identifier):triggerTime(triggerTime),visibleEnergy(visibleEnergy),identifier(identifier){
    
  }

  template<class T>
  double Event<T>::getTriggerTime() const{
    
    return triggerTime;

  }

  template<class T>
  T Event<T>::getVisibleEnergy() const{
    
    return visibleEnergy;

  }

  template<class T>
  unsigned Event<T>::getIdentifier() const{
    
    return identifier;

  }
    
  template <class T>
  double Event<T>::getTimeCorrelation(const Event<T>& other) const{

    return std::abs(this->triggerTime - other.triggerTime);
  
  }
  
    
  template <class T>
  bool Event<T>::isTimeCorrelated(const Event<T>& other, const Bounds<double>& timeBounds) const{
    
    return timeBounds.contains(getTimeCorrelation(other));

  }
  
  template<class T>
  bool Event<T>::hasVisibleEnergyWithin(const Bounds<T>& energyBounds) const{
    
    return energyBounds.contains(visibleEnergy);

  }
  
  template<class T>
  void Event<T>::print(std::ostream& output, unsigned outputOffset) const{

    unsigned firstColumnWidth = 13;
    output<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Trigger time"<<": "<<std::setw(14)<<std::left<<triggerTime
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Energy"<<": "<<std::setw(3)<<std::left<<visibleEnergy
      <<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Identifier"<<": "<<std::setw(8)<<std::left<<identifier;
    
  }
  
  template<class T>
  bool Event<T>::isEqualTo(const Event<T>& other) const{

    return identifier == other.identifier;
    
  }
  
  template<class T>
  std::ostream& operator<<(std::ostream& output, const Event<T>& event){
    
    event.print(output, 0);//print to output
    return output;
    
  }

  template<class T>
  bool operator==(const Event<T>& event1, const Event<T>& event2){

    return event1.isEqualTo(event2);
    
  }
  
  template<class T>
  bool operator!=(const Event<T>& event1, const Event<T>& event2){

    return !(event1 == event2);
    
  }
    
  template <class T>
  double getTimeCorrelation(const Event<T>& single1, const Event<T>& single2){

    return single1.getTimeCorrelation(single2);
  
  }
    
  template <class T>
  bool areTimeCorrelated(const Event<T>& single1, const Event<T>& single2, const Bounds<double>& timeBounds){

    return single1.isTimeCorrelated(single2, timeBounds);
  
  }

}

#endif