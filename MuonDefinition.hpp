#ifndef COSMOGENIC_MUON_DEFINTION_H
#define COSMOGENIC_MUON_DEFINTION_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{

  template <class T>
  struct Entry;
  
  template <class T>
  class Muon;
  
  template <class T>
  class MuonDefinition{
    
    T IVChargeThreshold;//lower cut on the IV charge
    T energyThreshold;//visible ID energy
    T energyToIDChargeFactor;//conversion from (possibly) MeV to DUQ's
    
    T IDChargeThreshold;//computed from the other members
    T IDChargeToEnergyFactor;//computed from the other members
    void updateEnergyThreshold();
    void updateIDChargeThreshold();

  public:
    MuonDefinition() = default;
    MuonDefinition(T IVChargeThreshold, T energyThreshold, T energyToIDChargeFactor);
    T getIVChargeThreshold() const;
    T getEnergyToIDChargeFactor() const;
    T getEnergyThreshold() const;
    T getIDChargeThreshold() const;
    T getIDChargeToEnergyFactor() const;
    void setIVChargeThreshold(T IVChargeThreshold);
    void setEnergyToIDChargeFactor(T energyToIDChargeFactor);
    void setEnergyThreshold(T energyThreshold);
    void setIDChargeThreshold(T IDChargeThreshold);
    void setIDChargeToEnergyFactor(T IDChargeToEnergyFactor);
    bool tag(const Entry<T>& entry) const;
    bool tag(const Muon<T>& muon) const;
    T getVisibleEnergy(T IDCharge) const;
    T getVisibleEnergy(const Muon<T>& muon) const;
    T getIDCharge(T energy) const;
    
  };
  
  template <class T>
  void MuonDefinition<T>::updateEnergyThreshold(){
    
    energyThreshold = IDChargeThreshold * IDChargeToEnergyFactor;

  }
  
  template <class T>
  void MuonDefinition<T>::updateIDChargeThreshold(){
    
    IDChargeThreshold = energyThreshold * energyToIDChargeFactor;

  }
  
  template <class T>
  MuonDefinition<T>::MuonDefinition(T IVChargeThreshold, T energyThreshold,  T energyToIDChargeFactor)
  :IVChargeThreshold(IVChargeThreshold),energyThreshold(energyThreshold),energyToIDChargeFactor(energyToIDChargeFactor),IDChargeThreshold(energyThreshold * energyToIDChargeFactor), IDChargeToEnergyFactor(1/energyToIDChargeFactor){
    
    if(IVChargeThreshold < 0 || energyThreshold < 0 || energyToIDChargeFactor  <= 0){
      
      auto errorMessage = std::to_string(IVChargeThreshold)+"DUQ, "+std::to_string(energyThreshold)+"MeV and "+std::to_string(energyToIDChargeFactor)+"DUQ/MeV are invalid parameters for a muon definition.";
      throw std::invalid_argument(errorMessage);
      
    }
    
  }

  template <class T>
  T MuonDefinition<T>::getIVChargeThreshold() const{
    
    return IVChargeThreshold;

  }

  template <class T>
  T MuonDefinition<T>::getEnergyThreshold() const{
    
    return energyThreshold;

  }
  
  template <class T>
  T MuonDefinition<T>::getEnergyToIDChargeFactor() const{
    
    return energyToIDChargeFactor;

  }
  
  template <class T>
  T MuonDefinition<T>::getIDChargeThreshold() const{
    
    return IDChargeThreshold;

  }
  
  template <class T>
  T MuonDefinition<T>::getIDChargeToEnergyFactor() const{
    
    return IDChargeToEnergyFactor;

  }

  template <class T>
  void MuonDefinition<T>::setIVChargeThreshold(T IVChargeThreshold){
    
    if(IVChargeThreshold >= 0) this->IVChargeThreshold = IVChargeThreshold;
    else throw std::invalid_argument(std::to_string(IVChargeThreshold)+"DUQ is not a valid inner veto charge threshold.");

  }
  
  template <class T>
  void MuonDefinition<T>::setEnergyThreshold(T energyThreshold){

    if(energyThreshold >= 0){
      
      this->energyThreshold = energyThreshold;
      updateIDChargeThreshold();
      
    }
    else throw std::invalid_argument(std::to_string(energyThreshold)+"MeV is not a valid visible energy threshold.");

  }
  
  template <class T>
  void MuonDefinition<T>::setEnergyToIDChargeFactor(T energyToIDChargeFactor){

    if(energyToIDChargeFactor > 0){
      
      this->energyToIDChargeFactor = energyToIDChargeFactor;
      IDChargeToEnergyFactor = 1/energyToIDChargeFactor;
      updateIDChargeThreshold();
      
    }
    else throw std::invalid_argument(std::to_string(energyToIDChargeFactor)+"DUQ/MeV is not a valid energy to inner detector charge factor.");
  }
  
  template <class T>
  void MuonDefinition<T>::setIDChargeThreshold(T IDChargeThreshold){

    if(IDChargeThreshold >= 0){
      
      this->IDChargeThreshold = IDChargeThreshold;
      updateEnergyThreshold();
      
    }
    else throw std::invalid_argument(std::to_string(IVChargeThreshold)+"DUQ is not a valid inner detector charge threshold.");
    
  }
  
  template <class T>
  void MuonDefinition<T>::setIDChargeToEnergyFactor(T IDChargeToEnergyFactor){

    if(IDChargeToEnergyFactor > 0){
      
      this->IDChargeToEnergyFactor = IDChargeToEnergyFactor;
      energyToIDChargeFactor = 1/IDChargeToEnergyFactor;
      updateEnergyThreshold();
      
    }
    else throw std::invalid_argument(std::to_string(IDChargeToEnergyFactor)+"MeV/DUQ is not a valid inner detector charge to energy factor.");

  }
  
  template <class T>
  bool MuonDefinition<T>::tag(const Entry<T>& entry) const{

    return entry.innerVetoData.charge[0] > IVChargeThreshold && ( entry.energy > energyThreshold || entry.innerDetectorData.charge[0] > IDChargeThreshold);

  }
  
  template <class T>
  bool MuonDefinition<T>::tag(const Muon<T>& muon) const{

    return muon.getVetoCharge() > IVChargeThreshold && (getVisibleEnergy(muon) > energyThreshold || muon.getDetectorCharge() > IDChargeThreshold);

  }
  
  template <class T>
  T MuonDefinition<T>::getVisibleEnergy(T IDCharge) const{

    return IDCharge * IDChargeToEnergyFactor;

  }
  
  template <class T>
  T MuonDefinition<T>::getVisibleEnergy(const Muon<T>& muon) const{

    return getVisibleEnergy(muon.getDetectorCharge());

  }
  
  template <class T>
  T MuonDefinition<T>::getIDCharge(T energy) const{

    return energy * energyToIDChargeFactor;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const MuonDefinition<T>& muonDefinition){
    
    auto formerPrecision = output.precision();
    output<<std::fixed;
    
    output<<"\n"<<std::setw(26)<<std::left<<"IV Charge threshold"<<": "<<std::setw(8)<<std::right<<std::setprecision(0)<<muonDefinition.getIVChargeThreshold()<<"\n"
      <<std::setw(26)<<std::left<<"Energy threshold"<<": "<<std::setw(8)<<std::right<<muonDefinition.getEnergyThreshold()<<"\n"
      <<std::setw(26)<<std::left<<"Energy to ID charge factor"<<": "<<std::setw(8)<<std::right<<muonDefinition.getEnergyToIDChargeFactor()<<"\n"
      <<std::setw(26)<<std::left<<"ID charge threshold"<<": "<<std::setw(8)<<std::right<<muonDefinition.getIDChargeThreshold()<<"\n"
      <<std::setw(26)<<std::left<<"ID charge to energy factor"<<": "<<std::setw(8)<<std::right<<std::setprecision(6)<<muonDefinition.getIDChargeToEnergyFactor();
      
    output.unsetf(std::ios_base::floatfield);
    output<<std::setprecision(formerPrecision);
    
    return output;
    
  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, MuonDefinition<T>& muonDefinition){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::string regexString = "^(";
    for(unsigned k = 0; k < 2; ++k) regexString += number+")[:,](";//start with a number :, seprator
    regexString += number+")$";
    
    std::regex regex(regexString);
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) muonDefinition = std::move(MuonDefinition<T>(std::stod(regexMatches[1]), std::stod(regexMatches[2]), std::stod(regexMatches[3])));
    else throw std::invalid_argument(token+" cannot be parsed to build a muon defintion.");
    
    return input;
  
  }
  
}



#endif
