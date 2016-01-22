#ifndef COSMOGENIC_PULSE_SHAPE_INFORMATION_H
#define COSMOGENIC_PULSE_SHAPE_INFORMATION_H

#include <iomanip>
#include <stdexcept>

namespace CosmogenicHunter{

  template <class T>
  class PulseShapeInformation{
    
    T inconsistency;//minus log (pulse shape likelihood at reconstruction positon)
    T inconsistencyInChimney;//minus log (pulse shape likelihood in the chimney)
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    PulseShapeInformation();
    PulseShapeInformation(T inconsistency, T inconsistencyInChimney);
    T getInconsistency() const;
    T getInconsistencyInChimney() const;
    T getChimneyInconsistencyRatio() const;
    void setInconsistency(T inconsistency);
    void setInconsistencyInChimney(T inconsistencyInChimney);
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class T>
  template <class Archive>
  void PulseShapeInformation<T>::serialize(Archive& archive){
    
    archive(inconsistency, inconsistencyInChimney);

  }
  
  template <class T>
  PulseShapeInformation<T>::PulseShapeInformation():inconsistency(1),inconsistencyInChimney(std::numeric_limits<T>::max()){
    
  }
  
  template <class T>
  PulseShapeInformation<T>::PulseShapeInformation(T inconsistency, T inconsistencyInChimney)
  :inconsistency(inconsistency),inconsistencyInChimney(inconsistencyInChimney){
    
    if(inconsistency < 0 || inconsistencyInChimney < 0 || (inconsistency == 0 && inconsistencyInChimney == 0) ){//both inconsistencies cannot be zero at the same time
      
      auto errorMessage = std::to_string(inconsistency)+" and "+std::to_string(inconsistencyInChimney)+" are not a valid pulse shape inconsitencies.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T PulseShapeInformation<T>::getInconsistency() const{
    
    return inconsistency;

  }

  template <class T>
  T PulseShapeInformation<T>::getInconsistencyInChimney() const{
    
    return inconsistencyInChimney;

  }
  
  template <class T>
  T PulseShapeInformation<T>::getChimneyInconsistencyRatio() const{
    
    if(inconsistency > 0) return inconsistencyInChimney / inconsistency;
    else return std::numeric_limits<T>::max(); //both inconsistencies cannot be zero at the same time

  }
  
  template <class T>
  void PulseShapeInformation<T>::setInconsistency(T inconsistency){
    
    if(inconsistency >= 0) this->inconsistency = inconsistency;
    else throw std::invalid_argument(std::to_string(inconsistency)+" is not a valid pulse shape inconsistency.");

  }
  
  template <class T>
  void PulseShapeInformation<T>::setInconsistencyInChimney(T inconsistencyInChimney){
    
    if(inconsistencyInChimney >= 0) this->inconsistencyInChimney = inconsistencyInChimney;
    else throw std::invalid_argument(std::to_string(inconsistencyInChimney)+" is not a valid pulse shape inconsistency in the chimney.");
    
  }
  
  template <class T>
  void PulseShapeInformation<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    output<<std::setw(outputOffset)<<std::left<<" "<<std::setw(20)<<std::left<<"Inconsistency"<<": "<<std::setw(7)<<std::left<<inconsistency<<"\n"
      <<std::setw(outputOffset)<<std::left<<" "<<std::setw(20)<<std::left<<"Chimney inconsistency"<<": "<<std::setw(4)<<std::left<<inconsistencyInChimney;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const PulseShapeInformation<T>& pulseShapeInformation){
    
    pulseShapeInformation.print(output, 0);
    return output;
    
  }

}

#endif