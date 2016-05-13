#ifndef COSMOGENIC_VETO_H
#define COSMOGENIC_VETO_H

#include <memory>
#include <iostream>

namespace CosmogenicHunter{
  
  template <class T>//to veto CandidatePair<T>'s with the accuracy of type T
  class Veto{
    
  protected:
    std::string name;//name of the veto
    
  public:
    Veto(std::string name);
    Veto(const Veto& other) = default;
    Veto(Veto&& other) = default;
    Veto& operator = (const Veto& other) = default;
    Veto& operator = (Veto&& other) = default;
    virtual ~Veto() = default;//custom destructor implies to define (even if default-ed) all copy / move / assignement operations
    const std::string& getName() const;
    virtual bool veto(const Single<T>& single) const = 0;//tag or reject the single
    virtual bool veto(const CandidatePair<T>& candidatePair) const = 0;//tag or reject the pair (may call veto(single) on prompt and/or delayed)
    virtual std::unique_ptr<Veto<T>> clone() const = 0;
    virtual void print(std::ostream& output) const = 0;//needed to act as if 'operator<<' was virtual
    
  };
  
  template <class T>
  Veto<T>::Veto(std::string name):name(std::move(name)){
    
  }
  
  template <class T>
  const std::string& Veto<T>::getName() const{
    
    return name;
    
  }
  
  
}

#endif