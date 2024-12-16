#include "lifeevent.h"

LifeEvent::LifeEvent(int a,const std::string &desc):age(a),description(desc){}

std::string LifeEvent::getDescription() const {return description;}
int LifeEvent::getAge() const {return age;}

GenericEvent::GenericEvent(int age,const std::string &desc):LifeEvent(age,desc){}

void GenericEvent::applyEvent(int&,int&,int&,int&,int&,int&) {
    // Does nothing
}
