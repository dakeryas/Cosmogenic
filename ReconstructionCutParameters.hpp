#ifndef COSMOGENIC_RECONSTRUCTION_CUT_PARAMETERS_H
#define COSMOGENIC_RECONSTRUCTION_CUT_PARAMETERS_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{
  
  template <class T>
  class ReconstructionCutParameters{
    
    T minEnergy;//minimum Single's energy that can be kept as a valid Single (happens for inconsistency == 0) (otherwise they are tagged/rejected as they have minEnergy  > energy)
    T characteristicInconsistencyInverse;
    
  public:
    ReconstructionCutParameters() = default;
    ReconstructionCutParameters(T minEnergy, T characteristicInconsistency);
    T getMinEnergy() const;
    T getCharacteristicInconsistency() const;
    void setMinEnergy(T minEnergy);
    void setCharacteristicInconsistency(T characteristicInconsistency);
    void setParameters(T minEnergy, T characteristicInconsistency);
    bool tag(const Single<T>& single) const;
    
  };
  
  template <class T>
  ReconstructionCutParameters<T>::ReconstructionCutParameters(T minEnergy, T characteristicInconsistency)
  :minEnergy(minEnergy),characteristicInconsistencyInverse(1/characteristicInconsistency){
    
    if(minEnergy < 0 || characteristicInconsistency <= 0 ){
      
      auto errorMessage = std::to_string(minEnergy)+" MeV and "+std::to_string(characteristicInconsistency)+" are invalid reconstruction cut parameters.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T ReconstructionCutParameters<T>::getMinEnergy() const{
    
    return minEnergy;

  }

  template <class T>
  T ReconstructionCutParameters<T>::getCharacteristicInconsistency() const{
    
    return 1/characteristicInconsistencyInverse;

  }
  
  template <class T>
  void ReconstructionCutParameters<T>::setMinEnergy(T minEnergy){
    
    if(minEnergy >= 0) this->minEnergy = minEnergy;
    else throw std::invalid_argument(std::to_string(minEnergy)+" MeV is not a valid minimum energy for the reconstruction cut.");

  }
  
  template <class T>
  void ReconstructionCutParameters<T>::setCharacteristicInconsistency(T characteristicInconsistency){
    
    if(characteristicInconsistency > 0) this->characteristicInconsistencyInverse = 1/characteristicInconsistency;
    else throw std::invalid_argument(std::to_string(characteristicInconsistency)+" is not a valid characteristic inconsistency for the reconstruction cut.");

  }
  
  template <class T>
  void ReconstructionCutParameters<T>::setParameters(T minEnergy, T characteristicInconsistency){

    setMinEnergy(minEnergy);
    setCharacteristicInconsistency(characteristicInconsistency);

  }
  
  template <class T>
  bool ReconstructionCutParameters<T>::tag(const Single<T>& single) const{

    return minEnergy * std::exp(characteristicInconsistencyInverse * single.getPositionInformation().getInconsistency()) > single.getVisibleEnergy(); // the position inconsistency is too high for such a small energy

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const ReconstructionCutParameters<T>& reconstructionCutParameters){

    output<<std::setw(28)<<std::left<<"Min energy"<<": "<<std::setw(6)<<std::right<<reconstructionCutParameters.getMinEnergy()<<"\n"
      <<std::setw(28)<<std::left<<"Characteristic inconsistency"<<": "<<std::setw(6)<<std::right<<reconstructionCutParameters.getCharacteristicInconsistency();
      
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, ReconstructionCutParameters<T>& reconstructionCutParameters){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::regex regex("(^"+number+")[:,]("+number+"$)");//start with a number :, seprator and end with another number
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) reconstructionCutParameters.setParameters(std::stod(regexMatches[1]), std::stod(regexMatches[2]));
    else throw std::invalid_argument(token+" cannot be parsed to build reconstruction cut parameters.");
    
    return input;
  
  }
  
}

#endif