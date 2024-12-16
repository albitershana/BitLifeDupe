#include "familyfriend.h"

FamilyMember::FamilyMember(const std::string &n,const std::string &r,int rel,int a,int w,int g,bool d)
: name(n),role(r),relationship(clampStat(rel)),age(a),wealth(w),generosity(g),divorced(d){}

FriendMember::FriendMember(const std::string &n,int rel,int a)
: name(n),relationship(clampStat(rel)),age(a){}
