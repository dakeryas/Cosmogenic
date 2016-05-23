#ifndef COSMOGENIC_CANDIDATE_TREE_H
#define COSMOGENIC_CANDIDATE_TREE_H

#include "Cosmogenic/CandidatePair.hpp"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Shower.hpp"

namespace CosmogenicHunter{

  template <class T, class K>
  class CandidateTree{
    
    CandidatePair<T> candidatePair;//parent of the tree
    Window<Shower<Muon<K>, Single<T>>> muonShowers;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    CandidateTree() = default;
    CandidateTree(CandidatePair<T> candidatePair, Window<Shower<Muon<K>, Single<T>>> muonShowers);
    const CandidatePair<T>& getCandidatePair() const;
    const Window<Shower<Muon<K>, Single<T>>>& getMuonShowers() const;
    double getTimeCorrelationToLastMuon() const;
    bool isAfterMuon(double afterMuonTimeVeto) const;
    
  };
  
  template <class T, class K>
  template <class Archive>
  void CandidateTree<T,K>::serialize(Archive& archive){
    
    archive(candidatePair, muonShowers);

  }
  
  template <class T, class K>
  CandidateTree<T,K>::CandidateTree(CandidatePair<T> candidatePair, Window<Shower<Muon<K>, Single<T>>> muonShowers)
  :candidatePair(std::move(candidatePair)),muonShowers(std::move(muonShowers)){
    
  }

  template <class T, class K>
  const CandidatePair<T>& CandidateTree<T,K>::getCandidatePair() const{
    
    return candidatePair;

  }
  
  template <class T, class K>
  const Window<Shower<Muon<K>, Single<T>>>& CandidateTree<T,K>::getMuonShowers() const{
    
    return muonShowers;
    
  }
  
  template <class T, class K>
  double CandidateTree<T,K>::getTimeCorrelationToLastMuon() const{
    
    return CosmogenicHunter::getTimeCorrelation(candidatePair.getPrompt(), muonShowers.back().getInitiator());
    
  }
  
  template <class T, class K>
  bool CandidateTree<T,K>::isAfterMuon(double afterMuonTimeVeto) const{
    
    return getTimeCorrelationToLastMuon() < afterMuonTimeVeto;
    
  }
  
  
  template <class T, class K>
  std::ostream& operator<<(std::ostream& output, const CandidateTree<T,K>& candidatePairTree){
    
    output<<"\n"<<std::setw(16)<<std::left<<"Parent Candidate"<<": \n";
    candidatePairTree.getCandidatePair().print(output, 3);
    output<<"\n"<<std::setw(16)<<std::left<<"Muon showers"<<": \n";
    candidatePairTree.getMuonShowers().print(output, 3);
    return output;
    
  }

}


#endif