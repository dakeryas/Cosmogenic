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
    T getSpaceCorrelation() const;
    bool isTimeCorrelated(const Bounds<double>& timeBounds) const;
    bool isSpaceCorrelated(T maxDistance) const;
    bool isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const;
    bool isPoorlyReconstructed(const ReconstructionCutParameters<T>& reconstructionCutParameters) const;
    bool isStoppingMuon(T minChimneyInconsistencyRatio) const;
    bool isBufferMuon(const BufferMuonCutParameters<T>& bufferMuonCutParameters) const;
    bool isVetoed(const InnerVetoThreshold<T>& innerVetoThreshold) const;
    bool isCosmogenic(T cosmogenicLikelihoodThreshold) const;
    void print(std::ostream& output, unsigned outputOffset) const;
    
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
  T CandidatePair<T>::getSpaceCorrelation() const{

    return getSpaceCorrelation(prompt, delayed);
  
  }

  template <class T>
  bool CandidatePair<T>::isTimeCorrelated(const Bounds<double>& timeBounds) const{
    
    return areTimeCorrelated(prompt, delayed, timeBounds);

  }

  template <class T>
  bool CandidatePair<T>::isSpaceCorrelated(T maxDistance) const{

    return areSpaceCorrelated(prompt, delayed, maxDistance);
  
  }

  template <class T>
  bool CandidatePair<T>::isLightNoise(const LightNoiseCutParameters<T>& lightNoiseCutParameters) const{
  
    return prompt.isLightNoise(lightNoiseCutParameters) || delayed.isLightNoise(lightNoiseCutParameters);

  }

  template <class T>
  bool CandidatePair<T>::isPoorlyReconstructed(const ReconstructionCutParameters<T>& reconstructionCutParameters) const{

    return delayed.isPoorlyReconstructed(reconstructionCutParameters);
  
  }
  
  template <class T>
  bool CandidatePair<T>::isStoppingMuon(T minChimneyInconsistencyRatio) const{

    return prompt.getChimneyInconsistencyRatio() + delayed.getChimneyInconsistencyRatio() < minChimneyInconsistencyRatio;
  
  }

  template <class T>
  bool CandidatePair<T>::isVetoed(const InnerVetoThreshold<T>& innerVetoThreshold) const{
  
    return prompt.isVetoed(innerVetoThreshold);

  }
  
  template <class T>
  bool CandidatePair<T>::isBufferMuon(const BufferMuonCutParameters<T>& bufferMuonCutParameters) const{
  
    return prompt.isBufferMuon(bufferMuonCutParameters);

  }
  
  template <class T>
  bool CandidatePair<T>::isCosmogenic(T cosmogenicLikelihoodThreshold) const{

    return prompt.getCosmogenicLikelihood() > cosmogenicLikelihoodThreshold;
  
  }

  template <class T>
  void CandidatePair<T>::print(std::ostream& output, unsigned outputOffset) const{
    
    unsigned firstColumnWidth = 7;
    output<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Prompt"<<": \n";
    prompt.print(output, outputOffset + 3);
    output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(firstColumnWidth)<<std::left<<"Delayed"<<": \n";
    delayed.print(output, outputOffset + 3);
    
  }
  
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const CandidatePair<T>& candidatePair){
    
    candidatePair.print(output, 0);
    return output;
    
  }

}


#endif