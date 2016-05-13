#ifndef COSMOGENIC_LIGHT_NOISE_CUT_PARAMETERS_H
#define COSMOGENIC_LIGHT_NOISE_CUT_PARAMETERS_H

#include <iomanip>
#include <stdexcept>
#include <regex>
#include "Cosmogenic/Veto.hpp"

namespace CosmogenicHunter{
  
  template <class T>
  class LightNoiseCutParameters : public Veto<T>{

    T maxRMS, slopeRMS, maxDifference, maxRatio;
    double maxStartTimeRMS;
    
    bool veto(const ChargeInformation<T>& chargeInformation) const;
    
  public:
    LightNoiseCutParameters();
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
    bool veto(const Single<T>& single) const;
    bool veto(const CandidatePair<T>& candidatePair) const;
    std::unique_ptr<Veto<T>> clone() const;
    void print(std::ostream& output) const;
    
  };
  
  template <class T>
  LightNoiseCutParameters<T>::LightNoiseCutParameters()
  :LightNoiseCutParameters(std::numeric_limits<T>::max(), 0, std::numeric_limits<T>::max(), 1, std::numeric_limits<T>::max()){
    
  }

  template <class T>
  LightNoiseCutParameters<T>::LightNoiseCutParameters(T maxRMS, T slopeRMS, T maxDifference, T maxRatio, double maxStartTimeRMS)
  :Veto<T>("LightNoiseVeto"),maxRMS(maxRMS),slopeRMS(slopeRMS),maxDifference(maxDifference),maxRatio(maxRatio),maxStartTimeRMS(maxStartTimeRMS){
    
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
  bool LightNoiseCutParameters<T>::veto(const ChargeInformation<T>& chargeInformation) const{

    return chargeInformation.getDifference() > maxDifference || chargeInformation.getRatio() > maxRatio || (chargeInformation.getStartTimeRMS() > maxStartTimeRMS && (chargeInformation.getRMS() > (maxRMS - slopeRMS * chargeInformation.getStartTimeRMS())));

  }
  
  template <class T>
  bool LightNoiseCutParameters<T>::veto(const Single<T>& single) const{

    return veto(single.getChargeInformation());

  }
  
  template <class T>
  bool LightNoiseCutParameters<T>::veto(const CandidatePair<T>& candidatePair) const{

    return veto(candidatePair.getPrompt()) || veto(candidatePair.getDelayed());

  }
  
  template <class T>
  std::unique_ptr<Veto<T>> LightNoiseCutParameters<T>::clone() const{

    return std::make_unique<LightNoiseCutParameters<T>>(*this);

  }
  
  template <class T>
  void LightNoiseCutParameters<T>::print(std::ostream& output) const{
    
    int labelColumnWidth = 13;
    int dataColumnWidth = 6;
    
    output<<std::setw(labelColumnWidth)<<std::left<<"Max RMS"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxRMS<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Slope RMS"<<": "<<std::setw(dataColumnWidth)<<std::right<<slopeRMS<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Max Diff"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxDifference<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Max Ratio"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxRatio<<"\n"
      <<std::setw(labelColumnWidth)<<std::left<<"Max RMSTstart"<<": "<<std::setw(dataColumnWidth)<<std::right<<maxStartTimeRMS;

  }
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const LightNoiseCutParameters<T>& lightNoiseCutParameters){

    lightNoiseCutParameters.print(output);
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