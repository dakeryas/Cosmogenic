#ifndef COSMOGENIC_CHARGE_DATA_H
#define COSMOGENIC_CHARGE_DATA_H

#include <iomanip>

namespace CosmogenicHunter{
  
  template <class T>
  struct ChargeData{
    
    T RMS, difference, ratio, startTimeRMS;
    ChargeData() = default;
    
  };
  
  template <class T>
  std::ostream& operator<<(std::ostream& output, const ChargeData<T>& chargeData){
      
    auto formerPrecision = output.precision();
    output<<std::fixed;
    
    output<<std::setw(5)<<std::right<<chargeData.RMS<<" | "
      <<std::setw(7)<<std::right<<chargeData.difference<<" | "
      <<std::setw(3)<<std::right<<std::setprecision(2)<<chargeData.ratio<<" | "
      <<std::setw(3)<<std::right<<std::setprecision(formerPrecision)<<chargeData.startTimeRMS;
    return output;
      
  };
  
}

#endif