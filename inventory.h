#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

enum class HouseType {None, Apartment, SuburbanHome, Mansion};

struct Inventory {
    bool hasRing;
    bool hasHouse; HouseType houseType;
    bool hasCar; std::string carBrand; int carCondition; int carCost;
    Inventory():hasRing(false),hasHouse(false),houseType(HouseType::None),hasCar(false),carBrand("None"),carCondition(0),carCost(0){}
};

#endif
