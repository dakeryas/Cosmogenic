#ifndef CANDIDATE_TREE_H
#define CANDIDATE_TREE_H

#include "Cosmogenic/Single.hpp"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Shower.hpp"

namespace CosmogenicHunter{

  template <class T, class K>
  class CandidateTree{
    
    Single<T> candidate;//parent of the tree
    Window<Shower<Muon<K>, Single<K>>> muonShowers;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    CandidateTree() = default;
    CandidateTree(Single<T> candidate, Window<Shower<Muon<K>, Single<K>>> muonShowers);
    const Single<T>& getCandidate() const;
    const Window<Shower<Muon<K>, Single<K>>>& getMuonShowers() const;
    
  };
  
  template <class T, class K>
  template <class Archive>
  void CandidateTree<T,K>::serialize(Archive& archive){
    
    archive(candidate, muonShowers);

  }
  
  template <class T, class K>
  CandidateTree<T,K>::CandidateTree(Single<T> candidate, Window<Shower<Muon<K>, Single<K>>> muonShowers)
  :candidate(std::move(candidate)),muonShowers(std::move(muonShowers)){
    
  }

  template <class T, class K>
  const Single<T>& CandidateTree<T,K>::getCandidate() const{
    
    return candidate;

  }
  
  template <class T, class K>
  const Window<Shower<Muon<K>, Single<K>>>& CandidateTree<T,K>::getMuonShowers() const{
    
    return muonShowers;
    
  }
  
  template <class T, class K>
  std::ostream& operator<<(std::ostream& output, const CandidateTree<T,K>& candidateTree){
    
    output<<"\n"<<std::setw(16)<<std::left<<"Parent Candidate"<<": \n";
    candidateTree.getCandidate().print(output, 3);
    output<<"\n"<<std::setw(16)<<std::left<<"Muon showers"<<": \n";
    candidateTree.getMuonShowers().print(output, 3);
    return output;
    
  }

}


#endif