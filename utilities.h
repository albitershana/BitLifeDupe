#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <iostream>
#include <cstdlib>

extern const int MAX_STAT;
extern const int MIN_STAT;

int getRandomNumber(int min,int max);
int clampStat(int v);
bool promptYesNo(const std::string &question);
std::string getRandomName();

#endif
