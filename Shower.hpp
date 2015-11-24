#ifndef COSMOGENIC_SHOWER_H
#define COSMOGENIC_SHOWER_H

#include "Cosmogenic/Window.hpp"

namespace CosmogenicHunter{

  template <class Initiator, class Follower>
  class Shower{
    
    Initiator initiator;//Initiator creating the follower flux
    Window<Follower> followerWindow;
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive);
    
  public:
    Shower() = default;
    Shower(Initiator initiator, double maximumDuration);//opens a window starting at Initiator.getTriggerTime() and lasting 'maximumDuration' to push followers
    const Initiator& getInitiator() const;
    double getTriggerTime() const;//returns Initiator.getTriggerTime() 
    const Window<Follower>& getFollowerWindow() const;
    unsigned getNumberOfFollowers() const;
    template <class... Args>
    void emplaceFollower(Args&&... args);
    void pushBackFollower(const Follower& follower);
    void pushBackFollower(Follower&& follower);
    void print(std::ostream& output, unsigned outputOffset) const;
    
  };
  
  template <class Initiator, class Follower>
  template <class Archive>
  void Shower<Initiator, Follower>::serialize(Archive& archive){
    
    archive(initiator, followerWindow);

  }
  
  template <class Initiator, class Follower>
  Shower<Initiator, Follower>::Shower(Initiator initiator, double maximumDuration):initiator(std::move(initiator)), followerWindow(initiator.getTriggerTime(), maximumDuration){
    
  }

  template <class Initiator, class Follower>
  const Initiator& Shower<Initiator, Follower>::getInitiator() const{
    
    return initiator;

  }
  
  template <class Initiator, class Follower>
  double Shower<Initiator, Follower>::getTriggerTime() const{

    return initiator.getTriggerTime();
    
  }

  template <class Initiator, class Follower>
  const Window<Follower>& Shower<Initiator, Follower>::getFollowerWindow() const{
    
    return followerWindow;

  }
  
  template <class Initiator, class Follower>
  unsigned Shower<Initiator, Follower>::getNumberOfFollowers() const{
    
    return followerWindow.getNumberOfEvents();

  }
  
  template <class Initiator, class Follower>
  template <class... Args>
  void Shower<Initiator, Follower>::emplaceFollower(Args&&... args){

    followerWindow.emplaceEvent(std::forward<Args>(args)...);

  }
  
  template <class Initiator, class Follower>
  void Shower<Initiator, Follower>::pushBackFollower(const Follower& follower){

    followerWindow.pushBackEvent(follower);
    
  }
  
  template <class Initiator, class Follower>
  void Shower<Initiator, Follower>::pushBackFollower(Follower&& follower){

    followerWindow.pushBackEvent(std::move(follower));//keep the r-value character with std::move
    
  }
  
  template <class Initiator, class Follower>
  void Shower<Initiator, Follower>::print(std::ostream& output, unsigned outputOffset) const{

    output<<std::setw(outputOffset)<<std::left<<""<<std::setw(9)<<std::left<<"Initiator"<<":\n";
    initiator.print(output, outputOffset + 3);//offset the initiator by 3 spaces
    
    if(!followerWindow.isEmpty()){
      
      output<<"\n"<<std::setw(outputOffset)<<std::left<<""<<std::setw(10)<<std::left<<"Followers"<<":\n";
      followerWindow.print(output, outputOffset + 3);
      
    }
    
  }
  
  template <class Initiator, class Follower>
  std::ostream& operator<<(std::ostream& output, const Shower<Initiator,Follower>& shower){
    
    shower.print(output, 0);
    return output;
    
  }

}



#endif