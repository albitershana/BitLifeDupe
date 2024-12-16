#include "utilities.h"
#include <ctime>

const int MAX_STAT = 100;
const int MIN_STAT = 0;

int getRandomNumber(int min,int max){
    return rand()%(max-min+1)+min;
}

int clampStat(int v) {
    if(v>MAX_STAT)v=MAX_STAT;
    if(v<MIN_STAT)v=MIN_STAT;
    return v;
}

bool promptYesNo(const std::string &question) {
    std::cout<<"\n"<<question<<" (y/n): ";
    char c;std::cin>>c;
    return (c=='y'||c=='Y');
}

std::string getRandomName() {
    const char* names[]={"Alex","Jordan","Taylor","Sam","Chris","Morgan","Casey","Robin","Jamie","Drew","Avery","Charlie","Riley","Cameron","Dakota","Reese"};
    int idx=getRandomNumber(0,15);
    return names[idx];
}
