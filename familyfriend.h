#ifndef FAMILYFRIEND_H
#define FAMILYFRIEND_H

#include <string>
#include "utilities.h"

class FamilyMember {
public:
    std::string name;
    std::string role;
    int relationship;
    int age;
    int wealth;
    int generosity;
    bool divorced;
    FamilyMember(const std::string &n,const std::string &r,int rel,int a=0,int w=0,int g=50,bool d=false);
};

class FriendMember {
public:
    std::string name;
    int relationship;
    int age;
    FriendMember(const std::string &n,int rel,int a=0);
};

#endif
