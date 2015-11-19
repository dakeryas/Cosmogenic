#ifndef NEUTRON_H
#define NEUTRON_H

#include "Cosmogenic/Event.hpp"
#include "Cosmogenic/Point.hpp"

namespace CosmogenicHunter{

  template <class T>
  class Neutron : public Event<T>{
    
    Point<T> position;//RecoBAMA reconstructed positon
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Neutron() = default;
    Neutron(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position);
    const Point<T>& getPosition() const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void Neutron<T>::serialize(Archive& archive){
    
    archive(cereal::base_class<Event<T>>(this), position);

  }
  
  template <class T>
  Neutron<T>::Neutron(double triggerTime, T vetoCharge, T visibleEnergy, unsigned identifier, Point<T> position)
  :Event<T>(triggerTime, vetoCharge, visibleEnergy, identifier), position(std::move(position)){
    
  }

  template <class T>
  const Point<T>& Neutron<T>::getPosition() const{
    
    return position;

  }
  
  template <class T>
  void Neutron<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    Event<T>::print(output, outputOffset);//print the base class
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(12)<<std::left<<"Position"<<": "<<position;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const Neutron<T> neutron){
    
    neutron.print(output, 0);//print the base class
    return output;
    
  }

}

#endif