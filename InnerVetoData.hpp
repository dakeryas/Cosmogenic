#ifndef COSMOGENIC_INNER_VETO_DATA_H
#define COSMOGENIC_INNER_VETO_DATA_H

#include <iomanip>

namespace CosmogenicHunter{
  
  template <class T>
  struct InnerVetoData{
    
    T charge;
    unsigned short numberOfHitPMTs;
    InnerVetoData() = default;
    
  };
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const InnerVetoData<T>& innerVetoData){
      
    output<<std::fixed;
    
    output<<std::setw(9)<<std::right<<innerVetoData.charge<<" | "
      <<std::setw(3)<<std::right<<innerVetoData.numberOfHitPMTs;
      
    return output;
    
  };
  
}

#endif