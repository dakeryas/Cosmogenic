#ifndef COSMOGENIC_LIGHT_NOISE_CUT_PARAMETERS_H
#define COSMOGENIC_LIGHT_NOISE_CUT_PARAMETERS_H

#include <iomanip>
#include <stdexcept>
#include <regex>

namespace CosmogenicHunter{
  
  template <class T>
  struct ChargeData;
  
  template <class T>
  class ChargeInformation;

  template <class T>
  class LightNoiseCutParameters{

    T maxRMS, slopeRMS, maxDifference, maxRatio;
    double maxStartTimeRMS;
    
  public:
    LightNoiseCutParameters() = default;
    LightNoiseCutParameters(T maxRMS, T slopeRMS, T maxDifference, T maxRatio, double maxStartTimeRMS);
    T getMaxRMS() const;
    T getSlopeRMS() const;
    T getMaxDifference() const;
    T getMaxRatio() const;
    double getMaxStartTimeRMS() const;
    void setMaxRMS(T maxRMS);
    void setSlopeRMS(T slopeRMS);
    void setMaxDifference(T maxDifference);
    void setMaxRatio(T maxRatio);
    void setMaxStartTimeRMS(double maxStartTimeRMS);
    void setParameters(T maxRMS, T slopeRMS, T maxDifference, T maxRatio, double maxStartTimeRMS);
    bool tag(const ChargeData<T>& chargeData) const;
    bool tag(const ChargeInformation<T>& chargeInformation) const;
    
  };

  template <class T>
  LightNoiseCutParameters<T>::LightNoiseCutParameters(T maxRMS, T slopeRMS, T maxDifference, T maxRatio, double maxStartTimeRMS)
  :maxRMS(maxRMS),slopeRMS(slopeRMS),maxDifference(maxDifference),maxRatio(maxRatio),maxStartTimeRMS(maxStartTimeRMS){
    
    if(maxRMS < 0 || slopeRMS < 0 || maxDifference  < 0 || maxRatio < 0 || maxRatio > 1 || maxStartTimeRMS <  0) throw std::invalid_argument("Invalid light noise parameters.");
    
  }

  template <class T>
  T LightNoiseCutParameters<T>::getMaxRMS() const{
    
    return maxRMS;

  }

  template <class T>
  T LightNoiseCutParameters<T>::getSlopeRMS() const{
    
    return slopeRMS;

  }
  
  template <class T>
  T LightNoiseCutParameters<T>::getMaxDifference() const{
    
    return maxDifference;

  }

  template <class T>
  T LightNoiseCutParameters<T>::getMaxRatio() const{
    
    return maxRatio;

  }
  
  template <class T>
  double LightNoiseCutParameters<T>::getMaxStartTimeRMS() const{
    
    return maxStartTimeRMS;

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setMaxRMS(T maxRMS){
    
    if(maxRMS >= 0) this->maxRMS = maxRMS;
    else throw std::invalid_argument(std::to_string(maxRMS)+"DUQ is not a valid light noise RMS charge.");

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setSlopeRMS(T slopeRMS){

    if(slopeRMS >= 0) this->slopeRMS = slopeRMS;
    else throw std::invalid_argument(std::to_string(slopeRMS)+"DUQ/ns is not a valid light noise RMS charge slope.");

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setMaxDifference(T maxDifference){
    
    if(maxDifference >= 0) this->maxDifference = maxDifference;
    else throw std::invalid_argument(std::to_string(maxDifference)+"DUQ is not a valid light noise charge difference.");

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setMaxRatio(T maxRatio){

    if(maxRatio >= 0 && maxRatio <= 1) this->maxRatio = maxRatio;
    else throw std::invalid_argument(std::to_string(maxRatio)+" is not a valid light noise charge ratio.");

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setMaxStartTimeRMS(double maxStartTimeRMS){

    if(maxStartTimeRMS > 0) this->maxStartTimeRMS = maxStartTimeRMS;
    else throw std::invalid_argument(std::to_string(maxStartTimeRMS)+"ns is not a valid light noise RMS start time.");

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::setParameters(T maxRMS, T slopeRMS, T maxDifference, T maxRatio, double maxStartTimeRMS){

    setMaxRMS(maxRMS);
    setSlopeRMS(slopeRMS);
    setMaxDifference(maxDifference);
    setMaxRatio(maxRatio);
    setMaxStartTimeRMS(maxStartTimeRMS);

  }
  
  template <class T>
  bool LightNoiseCutParameters<T>::tag(const ChargeData<T>& chargeData) const{

    return chargeData.difference > maxDifference || chargeData.ratio > maxRatio || (chargeData.startTimeRMS > maxStartTimeRMS && (chargeData.RMS > (maxRMS - slopeRMS * chargeData.startTimeRMS)));

  }
  
  template <class T>
  bool LightNoiseCutParameters<T>::tag(const ChargeInformation<T>& chargeInformation) const{

    return chargeInformation.getDifference() > maxDifference || chargeInformation.getRatio() > maxRatio || (chargeInformation.getStartTimeRMS() > maxStartTimeRMS && (chargeInformation.getRMS() > (maxRMS - slopeRMS * chargeInformation.getStartTimeRMS())));

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const LightNoiseCutParameters<T>& lightNoiseCutParameters){

    output<<std::setw(13)<<std::left<<"Max RMS"<<": "<<std::setw(6)<<std::right<<lightNoiseCutParameters.getMaxRMS()<<"\n"
      <<std::setw(13)<<std::left<<"Slope RMS"<<": "<<std::setw(6)<<std::right<<lightNoiseCutParameters.getSlopeRMS()<<"\n"
      <<std::setw(13)<<std::left<<"Max Diff"<<": "<<std::setw(6)<<std::right<<lightNoiseCutParameters.getMaxDifference()<<"\n"
      <<std::setw(13)<<std::left<<"Max Ratio"<<": "<<std::setw(6)<<std::right<<lightNoiseCutParameters.getMaxRatio()<<"\n"
      <<std::setw(13)<<std::left<<"Max RMSTstart"<<": "<<std::setw(6)<<std::right<<lightNoiseCutParameters.getMaxStartTimeRMS();
      
    return output;

  }
  
  template <class T>
  std::istream& operator>>(std::istream& input, LightNoiseCutParameters<T>& lightNoiseCutParameters){
  
    std::string token;
    input >> token;
    
    std::string number("[+-]?(?:\\d*\\.)?\\d+(?:[eE][-+]?[0-9]+)?");//decimal number with possible sign and exponent
    std::string regexString = "^(";
    for(unsigned k = 0; k < 4; ++k) regexString += number+")[:,](";//start with a number :, seprator
    regexString += number+")$";
    
    std::regex regex(regexString);
    std::smatch regexMatches;
    if(std::regex_search(token, regexMatches, regex)) lightNoiseCutParameters.setParameters(std::stod(regexMatches[1]), std::stod(regexMatches[2]), std::stod(regexMatches[3]), std::stod(regexMatches[4]), std::stod(regexMatches[5]));
    else throw std::invalid_argument(token+" cannot be parsed to build light noise cut parameters.");
    
    return input;
  
  }
  
}

#endif