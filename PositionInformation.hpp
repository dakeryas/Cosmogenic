#ifndef COSMOGENIC_POSITION_INFORMATION_H
#define COSMOGENIC_POSITION_INFORMATION_H

#include "Point.hpp"

namespace CosmogenicHunter{

  template <class T>
  class PositionInformation{
    
    Point<T> position;
    T inconsistency;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    PositionInformation();
    PositionInformation(Point<T> position, T inconsistency);
    const Point<T>& getPosition() const;
    T getInconsistency() const;
    void setPosition(Point<T> position);
    void setInconsistency(T inconsistency);
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void PositionInformation<T>::serialize(Archive& archive){
    
    archive(position, inconsistency);

  }
  
  template <class T>
  PositionInformation<T>::PositionInformation():inconsistency(0){
    
  }
  
  template <class T>
  PositionInformation<T>::PositionInformation(Point<T> position, T inconsistency)
  :position(std::move(position)),inconsistency(inconsistency){
    
    if(inconsistency < 0) throw std::invalid_argument(std::to_string(inconsistency)+" is not a valid position inconsistency.");
    
  }

  template <class T>
  const Point<T>& PositionInformation<T>::getPosition() const{
    
    return position;

  }

  template <class T>
  T PositionInformation<T>::getInconsistency() const{
    
    return inconsistency;

  }
  
  template <class T>
  void PositionInformation<T>::setPosition(Point<T> position){
    
    this->position = std::move(position);

  }
  
  template <class T>
  void PositionInformation<T>::setInconsistency(T inconsistency){
    
    if(inconsistency >= 0) this->inconsistency = inconsistency;
    else throw std::invalid_argument(std::to_string(inconsistency)+" is not a valid position inconsistency.");
    
  }
  
  template <class T>
  void PositionInformation<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    output<<std::setw(outputOffset)<<std::left<<" "<<std::setw(13)<<std::left<<"Position"<<": "<<std::setw(7)<<std::left<<position<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(13)<<std::left<<"Inconsistency"<<": "<<std::setw(4)<<std::left<<inconsistency;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const PositionInformation<T>& positionInformation){
    
    positionInformation.print(output, 0);
    return output;
    
  }

}

#endif