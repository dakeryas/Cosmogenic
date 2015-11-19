#ifndef CANDIDATE_TREE_H
#define CANDIDATE_TREE_H

#include "Cosmogenic/Candidate.hpp"
#include "Cosmogenic/Muon.hpp"
#include "Cosmogenic/Neutron.hpp"
#include "Cosmogenic/Shower.hpp"

namespace CosmogenicHunter{

  template <class T, class K>
  class CandidateTree{
    
    Candidate<T> candidate;//parent of the tree
    Window<Shower<Muon<K>, Neutron<K>>> muonShowers;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    CandidateTree() = default;
    CandidateTree(Candidate<T> candidate, Window<Shower<Muon<K>, Neutron<K>>> muonShowers);
    const Candidate<T>& getCandidate() const;
    const Window<Shower<Muon<K>, Neutron<K>>>& getMuonShowers() const;
    
  };
  
  template <class T, class K>
  template <class Archive>
  void CandidateTree<T,K>::serialize(Archive& archive){
    
    archive(candidate, muonShowers);

  }
  
  template <class T, class K>
  CandidateTree<T,K>::CandidateTree(Candidate<T> candidate, Window<Shower<Muon<K>, Neutron<K>>> muonShowers)
  :candidate(std::move(candidate)),muonShowers(std::move(muonShowers)){
    
  }

  template <class T, class K>
  const Candidate<T>& CandidateTree<T,K>::getCandidate() const{
    
    return candidate;

  }
  
  template <class T, class K>
  const Window<Shower<Muon<K>, Neutron<K>>>& CandidateTree<T,K>::getMuonShowers() const{
    
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