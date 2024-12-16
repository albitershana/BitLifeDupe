#ifndef LIFEEVENT_H
#define LIFEEVENT_H

#include <string>

class LifeEvent {
protected:
    int age;
    std::string description;
public:
    LifeEvent(int age,const std::string &desc);
    virtual ~LifeEvent()=default;
    virtual void applyEvent(int &health,int &happiness,int &intelligence,int &looks,int &wealth,int &education)=0;
    std::string getDescription() const;
    int getAge() const;
};

class GenericEvent:public LifeEvent {
public:
    GenericEvent(int age,const std::string &desc);
    void applyEvent(int&,int&,int&,int&,int&,int&) override;
};

#endif
