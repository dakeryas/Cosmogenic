#ifndef COSMOGENIC_CANDIDATE_PAIR_H
#define COSMOGENIC_CANDIDATE_PAIR_H

#include "Cosmogenic/Single.hpp"

namespace CosmogenicHunter{

  template <class T>
  class CandidatePair{
    
    Single<T> prompt;
    Single<T> delayed;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    CandidatePair() = default;
    CandidatePair(Single<T> prompt, Single<T> delayed);
    const Single<T>& getPrompt() const;
    const Single<T>& getDelayed() const;
    double getTimeCorrelation() const;
    double getSpaceCorrelation() const;
    bool isTimeCorrelated(double minTime, double maxTime) const;
    bool isSpaceCorrelated(double maxDistance) const;
    bool isLightNoise(T maxChargeRMS, T slopeRMS, T maxChargeDiff, T maxChargeRatio, double maxStartTimeRMS) const;
    bool isStoppingMuon(T scale, T slope) const;
    bool triggersInnerVeto(T maxInnerVetoCharge) const;
    
  };
  
  template <class T>
  template <class Archive>
  void CandidatePair<T>::serialize(Archive& archive){
    
    archive(prompt, delayed);

  }
  
  template <class T>
  CandidatePair<T>::CandidatePair(Single<T> prompt, Single<T> delayed)
  :prompt(std::move(prompt)),delayed(std::move(delayed)){
    
  }

  template <class T>
  const Single<T>& CandidatePair<T>::getPrompt() const{
    
    return prompt;

  }
  
  template <class T>
  const Single<T>& CandidatePair<T>::getDelayed() const{
    
    return delayed;
    
  }
  
  template <class T>
  double CandidatePair<T>::getTimeCorrelation() const{

    return delayed.getTriggerTime() - prompt.getTriggerTime();
  
  }

  template <class T>
  double CandidatePair<T>::getSpaceCorrelation() const{

    return getDistanceBetween(prompt.getPosition(), delayed.getPosition());
  
  }

  template <class T>
  bool CandidatePair<T>::isTimeCorrelated(double minTime, double maxTime) const{
    
    return areTimeCorrelated(prompt, delayed, minTime, maxTime);

  }

  template <class T>
  bool CandidatePair<T>::isSpaceCorrelated(double maxDistance) const{

    return areSpaceCorrelated(prompt, delayed, maxDistance);
  
  }

  template <class T>
  bool CandidatePair<T>::isLightNoise(T maxChargeRMS, T slopeRMS, T maxChargeDiff, T maxChargeRatio, double maxStartTimeRMS) const{
  
    return prompt.isLightNoise(maxChargeRMS, slopeRMS, maxChargeDiff, maxChargeRatio, maxStartTimeRMS) && delayed.isLightNoise(maxChargeRMS, slopeRMS, maxChargeDiff, maxChargeRatio, maxStartTimeRMS);

  }

  template <class T>
  bool CandidatePair<T>::isStoppingMuon(T scale, T slope) const{

    return delayed.getEnergy() < scale * std::exp(slope * delayed.getReconstructionGoodness());
  
  }

  template <class T>
  bool CandidatePair<T>::triggersInnerVeto(T maxInnerVetoCharge) const{
  
    return prompt.triggersInnerVeto(maxInnerVetoCharge) && delayed.triggersInnerVeto(maxInnerVetoCharge);

  }

  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const CandidatePair<T>& candidatePair){
    
    output<<std::setw(6)<<std::left<<"Prompt"<<": \n";
    candidatePair.getPrompt().print(output, 3);
    output<<"\n"<<std::setw(6)<<std::left<<"Delayed"<<": \n";
    candidatePair.getDelayed().print(output, 3);
    return output;
    
  }

  }


#endif