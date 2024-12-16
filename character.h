#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <memory>
#include "utilities.h"
#include "lifeevent.h"
#include "familyfriend.h"
#include "inventory.h"

class FamilyMember;
class FriendMember;

class Character {
private:
    std::string name;
    int age;
    int health;
    int happiness;
    int intelligence;
    int looks;
    int wealth;
    int education;

    std::string career;
    bool hasChosenCareer;
    bool married;
    bool hasDriversLicense;
    bool tookDriversTest;

    bool inCollege;
    std::string collegeMajor;
    int collegeYearsNeeded;
    int collegeYearsCompleted;

    bool fired;
    int baseSalary;
    int jobStress;
    int bossRelationship;
    int criminalNotoriety;

    Inventory inventory;
    std::vector<std::shared_ptr<LifeEvent>> lifeEvents;
    std::vector<FamilyMember> family;
    std::vector<FriendMember> friends;

    std::string deathReason;
    bool inheritedLife;
    bool died;

    int lastFriendRequestYear;
    int lastLoverRequestYear;

    bool isCriminalCareer();
    void addLifeEvent(std::shared_ptr<LifeEvent> event);
    void familyInitialization();
    void startCollege();
    void finalizeCollegeYear();
    void applyCriminalCareerProgression();
    void applyLegitCareerProgression();
    void applyCareerProgression();
    FamilyMember* getPartner();
    void useDatingApp();
    void randomEventsYearly();
    void tryDriversLicense(); 
    void yearlyEvents(); 
    void possessionsMenu(); 
    void healthHappinessMenu();
    void schoolActionsMenu();
    void jobMenu();
    void loveMenu();
    void familyMenu();
    void friendsMenu();

public:
    Character(const std::string &n);

    bool isAlive() const;
    void displayStats() const;
    void displayLifeEvents() const;
    void ageOneYearAction();
    void showMainMenu();
};

#endif
