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
    bool isLightNoise(const LightNoiseVeto<T>& lightNoiseVeto) const;
    bool isPoorlyReconstructed(const ReconstructionVeto<T>& reconstructionVeto) const;
    bool isStoppingMuon(const ChimneyVeto<T>& chimneyVeto) const;
    bool isBufferMuon(const BufferMuonVeto<T>& bufferMuonVeto) const;
    bool isVetoed(const InnerVeto<T>& innerVeto) const;
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
  bool CandidatePair<T>::isLightNoise(const LightNoiseVeto<T>& lightNoiseVeto) const{
  
    return lightNoiseVeto.veto(*this);

  }

  template <class T>
  bool CandidatePair<T>::isPoorlyReconstructed(const ReconstructionVeto<T>& reconstructionVeto) const{

    return reconstructionVeto.veto(*this);
  
  }
  
  template <class T>
  bool CandidatePair<T>::isStoppingMuon(const ChimneyVeto<T>& chimneyVeto) const{
    
    return chimneyVeto.veto(*this);
  
  }

  template <class T>
  bool CandidatePair<T>::isVetoed(const InnerVeto<T>& innerVeto) const{
  
    return innerVeto.veto(*this);

  }
  
  template <class T>
  bool CandidatePair<T>::isBufferMuon(const BufferMuonVeto<T>& bufferMuonVeto) const{
  
    return bufferMuonVeto.veto(*this);

  }
  
  template <class T>
  bool CandidatePair<T>::isCosmogenic(T cosmogenicLikelihoodThreshold) const{

    return prompt.isCosmogenic(cosmogenicLikelihoodThreshold);
  
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