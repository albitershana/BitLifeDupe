#include "character.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <ctime>

bool Character::isCriminalCareer() {
    return (career=="Mafia"||career=="Drug Dealer"||career=="Contract Killer"||career=="Cybercriminal"||
            career=="Pickpocket"||career=="Burglar"||career=="Armed Robber"||career=="Smuggler"||career=="Money Launderer"||career=="Stripper");
}

void Character::addLifeEvent(std::shared_ptr<LifeEvent> event) {
    lifeEvents.push_back(event);
    event->applyEvent(health,happiness,intelligence,looks,wealth,education);
    if(health<=0 && !died) {
        if(deathReason.empty()) deathReason="Severe health issues.";
        died=true;
    }
    if(happiness<=0 && !died) {
        if(deathReason.empty()) deathReason="Severe depression.";
        died=true;
    }
}

void Character::familyInitialization() {
    int p1Age=getRandomNumber(25,40);
    int p2Age=p1Age+getRandomNumber(-2,2);

    int p1Wealth=getRandomNumber(1000,100000);
    int p2Wealth=getRandomNumber(1000,100000);
    int p1Gen=getRandomNumber(30,100);
    int p2Gen=getRandomNumber(30,100);

    family.push_back(FamilyMember(getRandomName(),"Parent",80,p1Age,p1Wealth,p1Gen,false));
    family.push_back(FamilyMember(getRandomName(),"Parent",80,p2Age,p2Wealth,p2Gen,false));

    int numSiblings=getRandomNumber(0,3);
    for(int i=0;i<numSiblings;i++) {
        int sAge=age+getRandomNumber(-5,5);
        if(sAge<0)sAge=0;
        int sibWealth=0;
        int sibGen=getRandomNumber(30,70);
        family.push_back(FamilyMember(getRandomName(),"Sibling",getRandomNumber(50,80),sAge,sibWealth,sibGen,false));
    }
}

void Character::startCollege() {
    std::cout<<"\nYou turned 18. Attend college?\n";
    if(promptYesNo("Go to college?")) {
        inCollege=true;
        std::cout<<"\nMajors:\n1.CS(4y)\n2.Medicine(6y)\n3.Law(5y)\n4.Business(4y)\n5.Arts(3y)\n6.Engineering(4y)\n7.Education(4y)\n";
        int c;std::cin>>c;
        switch(c) {
            case 1: collegeMajor="Computer Science";collegeYearsNeeded=4;break;
            case 2: collegeMajor="Medicine";collegeYearsNeeded=6;break;
            case 3: collegeMajor="Law";collegeYearsNeeded=5;break;
            case 4: collegeMajor="Business";collegeYearsNeeded=4;break;
            case 5: collegeMajor="Arts";collegeYearsNeeded=3;break;
            case 6: collegeMajor="Engineering";collegeYearsNeeded=4;break;
            case 7: collegeMajor="Education";collegeYearsNeeded=4;break;
            default:collegeMajor="Undeclared";collegeYearsNeeded=4;break;
        }
        collegeYearsCompleted=0;
        addLifeEvent(std::make_shared<GenericEvent>(age,"Started college in "+collegeMajor));
        std::cout<<"\nYou started college in "<<collegeMajor<<".\n";
    } else {
        inCollege=false;
    }
}

void Character::finalizeCollegeYear() {
    if(died)return;
    if(inCollege && collegeYearsCompleted<collegeYearsNeeded) {
        if(education<65) {
            inCollege=false;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Kicked out of college(low performance)"));
            std::cout<<"\nYou got kicked out of college!\n";
        } else {
            collegeYearsCompleted++;
            int eduGain=getRandomNumber(5,10);
            education=clampStat(education+eduGain);
            intelligence=clampStat(intelligence+(eduGain/2));
            addLifeEvent(std::make_shared<GenericEvent>(age,"Completed a year of "+collegeMajor));
            std::cout<<"\nCompleted another year of "<<collegeMajor<<".\n";
            if(collegeYearsCompleted==collegeYearsNeeded) {
                inCollege=false;
                addLifeEvent(std::make_shared<GenericEvent>(age,"Graduated in "+collegeMajor+"!"));
                std::cout<<"\nYou graduated in "<<collegeMajor<<"!\n";
            }
        }
    }
}

void Character::applyCriminalCareerProgression() {
    if(died)return;
    int risk=getRandomNumber(1,100);
    if(risk<50) {
        int profit=getRandomNumber(1000,5000);
        addLifeEvent(std::make_shared<GenericEvent>(age,"Criminal earnings: +$"+std::to_string(profit)));
        wealth+=profit;
        std::cout<<"\nYou received $"<<profit<<" from criminal activities.\n";
        criminalNotoriety+=10;
    } else if(risk<80) {
        addLifeEvent(std::make_shared<GenericEvent>(age,"Close call with the law, happiness-10"));
        happiness-=10;
        std::cout<<"\nClose call with the law. Hap-10.\n";
        criminalNotoriety+=5;
    } else {
        if(getRandomNumber(1,100)<50) {
            deathReason="Caught by police in criminal act.";
            died=true;health=0;
        } else {
            deathReason="Killed by rivals in underworld.";
            died=true;health=0;
        }
    }
}

void Character::applyLegitCareerProgression() {
    if(died||fired||!hasChosenCareer||isCriminalCareer())return;
    int raiseChance=getRandomNumber(1,100);
    if(raiseChance<20) {
        int raiseAmt=getRandomNumber(2000,5000);
        baseSalary+=raiseAmt;
        addLifeEvent(std::make_shared<GenericEvent>(age,"Promotion +$"+std::to_string(raiseAmt)));
        std::cout<<"\nYou got a raise of $"<<raiseAmt<<".\n";
    }
    int income=baseSalary;
    int tax=(int)(income*0.2);
    wealth+=income-tax;
    addLifeEvent(std::make_shared<GenericEvent>(age,"Earned $"+std::to_string(income)+" paid $"+std::to_string(tax)+" tax."));
    std::cout<<"\nEarned $"<<income<<", paid $"<<tax<<" tax.\n";

    if(jobStress>40 && getRandomNumber(1,100)<(jobStress-40)) {
        int hLoss=getRandomNumber(-10,-1);
        health=clampStat(health+hLoss);
        addLifeEvent(std::make_shared<GenericEvent>(age,"Job stress hurt your health."));
        std::cout<<"\nJob stress affected your health.\n";
    }
}

void Character::applyCareerProgression() {
    if(!hasChosenCareer)return;
    if(isCriminalCareer()) applyCriminalCareerProgression();
    else applyLegitCareerProgression();
}

FamilyMember* Character::getPartner() {
    for(auto &f:family) {
        if(f.role=="Lover"||f.role=="Spouse") return &f;
    }
    return nullptr;
}

void Character::useDatingApp() {
    if(died)return;
    if(age<18||wealth<100) {
        std::cout<<"\nCannot use dating app now.\n";
        return;
    }
    wealth-=100;
    int partnerAge;std::cout<<"\nEnter desired partner age:";std::cin>>partnerAge;
    char g;std::cout<<"Enter desired partner gender(M/F):";std::cin>>g;
    std::string pname=getRandomName();
    int prel=getRandomNumber(60,80);
    family.push_back(FamilyMember(pname,"Lover",prel,partnerAge,0,50,false));
    addLifeEvent(std::make_shared<GenericEvent>(age,"Matched with "+pname+" via app."));
    std::cout<<"\nYou matched with "<<pname<<" and started dating!\n";
}

void Character::randomEventsYearly() {
    if(died)return;

    // parents divorce
    if(age<18) {
        int pCount=0;int pIndex[2];
        for(size_t i=0;i<family.size();i++) {
            if(family[i].role=="Parent" && !family[i].divorced) {
                if(pCount<2) pIndex[pCount]= (int)i;
                pCount++;
            }
        }
        if(pCount>=2 && getRandomNumber(1,100)<5) {
            family[pIndex[0]].divorced=true;
            family[pIndex[1]].divorced=true;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Your parents got divorced."));
            std::cout<<"\nYour parents got divorced!\n";
        }

        // new sibling
        if(age<10 && pCount>=2 && getRandomNumber(1,100)<10) {
            std::string sName=getRandomName();
            family.push_back(FamilyMember(sName,"Sibling",50,0,0,getRandomNumber(30,70),false));
            addLifeEvent(std::make_shared<GenericEvent>(age,"A new sibling "+sName+" was born!"));
            std::cout<<"\nA new sibling "<<sName<<" was born!\n";
        }
    }

    // friend request
    if((age - lastFriendRequestYear)>=5 && getRandomNumber(1,100)<50) {
        std::string fname=getRandomName();
        if(promptYesNo(fname+" wants to be your friend. Accept?")) {
            friends.push_back(FriendMember(fname,getRandomNumber(40,70),age));
            addLifeEvent(std::make_shared<GenericEvent>(age,"New friend: "+fname));
            std::cout<<"\nNow friends with "<<fname<<".\n";
        } else std::cout<<"\nYou declined.\n";
        lastFriendRequestYear=age;
    }

    // lover request if single
    if(age>=13 && getPartner()==nullptr && (age - lastLoverRequestYear)>=6 && getRandomNumber(1,100)<30) {
        std::string lname=getRandomName();
        if(promptYesNo(lname+" is interested in dating you. Accept?")) {
            family.push_back(FamilyMember(lname,"Lover",getRandomNumber(60,80),age,0,50,false));
            addLifeEvent(std::make_shared<GenericEvent>(age,"Started dating "+lname));
            std::cout<<"\nNow dating "<<lname<<".\n";
        } else std::cout<<"\nYou declined.\n";
        lastLoverRequestYear=age;
    }
}

void Character::tryDriversLicense() {
    if(!tookDriversTest&&(age==16||age==17)) {
        if(promptYesNo("Attempt license test?")) {
            tookDriversTest=true;
            std::cout<<"\nDriver's License Test:\n";
            std::cout<<"Q1: You see a Stop sign. You must:\nA)Stop completely\nB)Go\nC)Honk\n";
            char ans1;std::cin>>ans1;
            bool q1=(ans1=='A'||ans1=='a');
            std::cout<<"Q2: Speed limit is 50 mph, you're going 70, cop signals you:\nA)Stop\nB)Ignore\nC)Honk\n";
            char ans2;std::cin>>ans2;
            bool q2=(ans2=='A'||ans2=='a');
            std::cout<<"Q3: At a red light:\nA)Stop\nB)Go\nC)Honk\n";
            char ans3;std::cin>>ans3;
            bool q3=(ans3=='A'||ans3=='a');

            if(q1 && q2 && q3) {
                hasDriversLicense=true;
                addLifeEvent(std::make_shared<GenericEvent>(age,"Passed driving test!"));
                std::cout<<"\nYou got your license!\n";
            } else {
                addLifeEvent(std::make_shared<GenericEvent>(age,"Failed driving test."));
                std::cout<<"\nFailed test.\n";
            }
        }
    }
}

void Character::yearlyEvents() {
    if(died)return;
    applyCareerProgression();
    finalizeCollegeYear();
    // everyone ages
    for (auto &f:family) f.age++;
    for (auto &fr:friends) fr.age++;

    randomEventsYearly();
    if(inventory.hasHouse) happiness=clampStat(happiness+5);
}

void Character::possessionsMenu() {
    if(died)return;
    std::cout<<"\n--- Possessions Menu ---\n";
    std::cout<<"Wealth: $"<<wealth<<"\n";
    if(inventory.hasCar) std::cout<<"Car: "<<inventory.carBrand<<"(Cond:"<<inventory.carCondition<<")\n"; else std::cout<<"No Car.\n";
    if(inventory.hasHouse) {
        std::cout<<"House: ";
        if(inventory.houseType==HouseType::Apartment) std::cout<<"Apartment\n";
        else if(inventory.houseType==HouseType::SuburbanHome) std::cout<<"Suburban Home\n";
        else if(inventory.houseType==HouseType::Mansion) std::cout<<"Mansion\n";
    } else std::cout<<"No House.\n";
    if(inventory.hasRing) std::cout<<"Ring: Yes\n"; else std::cout<<"No Ring.\n";
    std::cout<<"\n1.Shop\n0.Back\n";
    int c;std::cin>>c;
    if(died)return;
    if(c==1) {
        std::cout<<"\n--- Shop ---\n";
        std::cout<<"1. Buy Ring($1000)\n2. Buy Car\n3. Buy House\n4. Sell Items\n0.Back\n";
        int sc;std::cin>>sc;
        if(died)return;
        switch(sc) {
            case 1:
                if(wealth>=1000 && !inventory.hasRing) {
                    wealth-=1000;inventory.hasRing=true;
                    addLifeEvent(std::make_shared<GenericEvent>(age,"Bought ring $1000"));
                    std::cout<<"\nYou bought a ring.\n";
                } else std::cout<<"\nNot enough or already have ring.\n";
                break;
            case 2:
                if(inventory.hasCar) {std::cout<<"\nAlready have car.\n"; break;}
                std::cout<<"\nCar:1.Used($1000)2.Sedan($5000)3.Sports($20000)0.Back\n";
                {
                    int cc;std::cin>>cc;
                    if(died)return;
                    int cost=0;int cond=0;std::string br="Car";
                    if(cc==1){cost=1000;cond=70;br="Used Car";}
                    else if(cc==2){cost=5000;cond=90;br="Sedan";}
                    else if(cc==3){cost=20000;cond=100;br="Sports Car";}
                    else break;
                    if(wealth>=cost) {wealth-=cost;inventory.hasCar=true;inventory.carBrand=br;inventory.carCondition=cond;inventory.carCost=cost;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Bought "+br+" $"+std::to_string(cost)));
                        std::cout<<"\nYou bought a "<<br<<" for $"<<cost<<".\n";
                    } else std::cout<<"\nNot enough money.\n";
                }
                break;
            case 3:
                if(inventory.hasHouse){std::cout<<"\nAlready have house.\n";break;}
                std::cout<<"\nHouse:1.Apt($20000)2.Suburban($100000)3.Mansion($500000)0.Back\n";
                {
                    int hc;std::cin>>hc;
                    if(died)return;
                    int cost=0;HouseType ht=HouseType::None;
                    if(hc==1){cost=20000;ht=HouseType::Apartment;}
                    else if(hc==2){cost=100000;ht=HouseType::SuburbanHome;}
                    else if(hc==3){cost=500000;ht=HouseType::Mansion;}
                    else break;
                    if(wealth>=cost) {
                        wealth-=cost;inventory.hasHouse=true;inventory.houseType=ht;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Bought house $"+std::to_string(cost)));
                        std::cout<<"\nYou bought a house for $"<<cost<<".\n";
                    } else std::cout<<"\nNot enough money.\n";
                }
                break;
            case 4:
                std::cout<<"\nSell items:\n";
                if(inventory.hasRing)std::cout<<"1.Ring($500)\n";
                if(inventory.hasCar)std::cout<<"2.Car(half)\n";
                if(inventory.hasHouse)std::cout<<"3.House(half)\n";
                std::cout<<"0.Back\n";
                {
                    int sel;std::cin>>sel;
                    if(died)return;
                    if(sel==1&&inventory.hasRing) {
                        wealth+=500;inventory.hasRing=false;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Sold ring $500"));
                        std::cout<<"\nSold ring $500.\n";
                    } else if(sel==2&&inventory.hasCar) {
                        int val=inventory.carCost/2;
                        wealth+=val;inventory.hasCar=false;inventory.carBrand="None";inventory.carCondition=0;inventory.carCost=0;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Sold car $"+std::to_string(val)));
                        std::cout<<"\nSold car for $"<<val<<"\n";
                    } else if(sel==3&&inventory.hasHouse) {
                        int val=0;
                        if(inventory.houseType==HouseType::Apartment) val=20000/2;
                        else if(inventory.houseType==HouseType::SuburbanHome) val=100000/2;
                        else if(inventory.houseType==HouseType::Mansion) val=500000/2;
                        wealth+=val;inventory.hasHouse=false;inventory.houseType=HouseType::None;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Sold house $"+std::to_string(val)));
                        std::cout<<"\nSold house $"<<val<<"\n";
                    } else if(sel!=0) {
                        std::cout<<"\nInvalid.\n";
                    }
                }
                break;
            default:break;
        }
    }
}

void Character::healthHappinessMenu() {
    if(died)return;
    std::cout<<"\n--- Health & Happiness Menu ---\n";
    std::cout<<"Health:\n1.Spa($200)+10h\n2.Doctor($500)+20h\n3.Massage($100)+5h\n";
    std::cout<<"Happiness:\n4.Walk(free)+5hap\n5.Club($100)+10hap\n6.Restaurant($50)+7hap\n7.Movie($30)+5hap\n0.Back\n";
    int c;std::cin>>c;
    if(died)return;
    switch(c) {
        case 1:
            if(wealth>=200) {wealth-=200;health=clampStat(health+10);addLifeEvent(std::make_shared<GenericEvent>(age,"Spa+health"));std::cout<<"\nSpa improved health.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        case 2:
            if(wealth>=500){wealth-=500;health=clampStat(health+20);addLifeEvent(std::make_shared<GenericEvent>(age,"Doctor+bighealth"));std::cout<<"\nDoctor improved health.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        case 3:
            if(wealth>=100){wealth-=100;health=clampStat(health+5);addLifeEvent(std::make_shared<GenericEvent>(age,"Massage+5health"));std::cout<<"\nMassage improved health.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        case 4:
            happiness=clampStat(happiness+5);addLifeEvent(std::make_shared<GenericEvent>(age,"Walk+5hap"));
            std::cout<<"\nWalk improved happiness.\n";
            break;
        case 5:
            if(wealth>=100){wealth-=100;happiness=clampStat(happiness+10);addLifeEvent(std::make_shared<GenericEvent>(age,"Club+hap"));std::cout<<"\nClub improved happiness.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        case 6:
            if(wealth>=50){wealth-=50;happiness=clampStat(happiness+7);addLifeEvent(std::make_shared<GenericEvent>(age,"Restaurant+hap"));std::cout<<"\nRestaurant improved happiness.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        case 7:
            if(wealth>=30){wealth-=30;happiness=clampStat(happiness+5);addLifeEvent(std::make_shared<GenericEvent>(age,"Movie+hap"));std::cout<<"\nMovie improved happiness.\n";}
            else std::cout<<"\nNot enough money.\n";
            break;
        default:break;
    }
}

void Character::schoolActionsMenu() {
    if(died)return;
    if(age>=18) {
        std::cout<<"\nYou are no longer in school.\n";
        return;
    }
    std::cout<<"\n--- School Menu ---\n";
    std::cout<<"1.Study(+Edu)\n2.Suck up(+Edu)\n3.Slack(-Edu)\n0.Back\n";
    int c;std::cin>>c;
    if(died)return;
    switch(c) {
        case 1:
            education=clampStat(education+getRandomNumber(2,5));
            intelligence=clampStat(intelligence+1);
            addLifeEvent(std::make_shared<GenericEvent>(age,"Studied hard."));
            std::cout<<"\nEducation improved.\n";
            break;
        case 2:
            education=clampStat(education+getRandomNumber(1,3));
            addLifeEvent(std::make_shared<GenericEvent>(age,"Sucked up to teacher."));
            std::cout<<"\nSmall edu gain.\n";
            break;
        case 3:
            education=clampStat(education-getRandomNumber(1,5));
            addLifeEvent(std::make_shared<GenericEvent>(age,"Slacked off in school."));
            std::cout<<"\nEdu decreased.\n";
            break;
        default:break;
    }
}

void Character::jobMenu() {
    if(died)return;
    if(age<18) {
        std::cout<<"\nToo young for a job.\n";
        return;
    }
    std::cout<<"\n--- Job Menu ---\n";
    if(!hasChosenCareer||career=="Unemployed"||fired) {
        // If graduated from college offer major job first
        if(!collegeMajor.empty() && !inCollege && collegeYearsCompleted==collegeYearsNeeded && !hasChosenCareer) {
            // major related:
            std::string majorJob;int majorSalary=0;
            if(collegeMajor=="Computer Science"){majorJob="Software Engineer";majorSalary=80000;}
            else if(collegeMajor=="Medicine"){majorJob="Doctor";majorSalary=120000;}
            else if(collegeMajor=="Law"){majorJob="Lawyer";majorSalary=100000;}
            else if(collegeMajor=="Business"){majorJob="Business Analyst";majorSalary=60000;}
            else if(collegeMajor=="Arts"){majorJob="Artist";majorSalary=40000;}
            else if(collegeMajor=="Engineering"){majorJob="Engineer";majorSalary=80000;}
            else if(collegeMajor=="Education"){majorJob="Teacher";majorSalary=40000;}
            std::cout<<"\nDegree in "<<collegeMajor<<": become "<<majorJob<<"($"<<majorSalary<<")\n";
            if(promptYesNo("Take this job?")) {
                career=majorJob;baseSalary=majorSalary;hasChosenCareer=true;fired=false;jobStress=40;bossRelationship=50;
                addLifeEvent(std::make_shared<GenericEvent>(age,"Got job as "+career));
                std::cout<<"\nYou got a job as "<<career<<".\n";
                return;
            } else {
                std::cout<<"\nYou declined. Choose from normal/unconventional:\n";
            }
        }
        // no major or declined major job:
        std::cout<<"Jobs:\n1.Office(40k)\n2.Sales(35k)\n3.Musician(30k)\n4.Drug Dealer\n5.Contract Killer\n6.Stripper\n0.Back\n";
        int ch;std::cin>>ch;
        if(died)return;
        if(ch==1){career="Office Worker";baseSalary=40000;fired=false;hasChosenCareer=true;jobStress=30;bossRelationship=50;}
        else if(ch==2){career="Sales";baseSalary=35000;fired=false;hasChosenCareer=true;jobStress=30;bossRelationship=50;}
        else if(ch==3){career="Musician";baseSalary=30000;fired=false;hasChosenCareer=true;jobStress=20;bossRelationship=50;}
        else if(ch==4){career="Drug Dealer";hasChosenCareer=true;fired=false;criminalNotoriety+=10;}
        else if(ch==5){career="Contract Killer";hasChosenCareer=true;fired=false;criminalNotoriety+=20;}
        else if(ch==6){career="Stripper";hasChosenCareer=true;fired=false;baseSalary=20000;jobStress=20;bossRelationship=50;}
        else return;
        addLifeEvent(std::make_shared<GenericEvent>(age,"Got a job as "+career));
        std::cout<<"\nYou got a job as "<<career<<".\n";
    } else if(isCriminalCareer()) {
        std::cout<<"Criminal career, no stable menu.\n";
    } else {
        // have job
        std::cout<<"Career:"<<career<<" Salary:$"<<baseSalary<<" BossRel:"<<bossRelationship<<"\n";
        std::cout<<"1.Ask promotion\n2.Insult boss\n3.Work more\n4.Quit\n0.Back\n";
        int c;std::cin>>c;
        if(died)return;
        switch(c) {
            case 1:
                if(bossRelationship>60 && looks>50) {
                    {
                        int raiseAmt=getRandomNumber(2000,5000);
                        baseSalary+=raiseAmt;
                        addLifeEvent(std::make_shared<GenericEvent>(age,"Promotion +$"+std::to_string(raiseAmt)));
                        std::cout<<"\nPromotion +$"<<raiseAmt<<".\n";
                    }
                } else {
                    bossRelationship-=10;
                    std::cout<<"\nNo promotion.\n";
                }
                break;
            case 2:
                addLifeEvent(std::make_shared<GenericEvent>(age,"Insulted boss."));
                bossRelationship-=30;
                if(bossRelationship<20) {
                    fired=true;career="Unemployed";baseSalary=0;
                    addLifeEvent(std::make_shared<GenericEvent>(age,"Boss fired you."));
                    std::cout<<"\nFired.\n";
                } else std::cout<<"\nBoss hates you more.\n";
                break;
            case 3:
                addLifeEvent(std::make_shared<GenericEvent>(age,"Worked more hours."));
                bossRelationship+=5;wealth+=2000;happiness-=5;health-=5;
                std::cout<<"\nEarned $2000, lost some health/happiness.\n";
                break;
            case 4:
                career="Unemployed";fired=true;baseSalary=0;
                addLifeEvent(std::make_shared<GenericEvent>(age,"You quit your job."));
                std::cout<<"\nYou quit.\n";
                break;
            default: break;
        }
    }
}

void Character::loveMenu() {
    if(died)return;
    std::cout<<"\n--- Love Menu ---\n";
    FamilyMember* partner=getPartner();
    if(partner) {
        std::cout<<"Partner: "<<partner->name<<" ("<<partner->role<<") Age:"<<partner->age<<" Rel:"<<partner->relationship<<"\n";
        if(partner->role=="Spouse") std::cout<<"(Married)\n";
        std::cout<<"\n1.Spend time\n2.Give gift\n";
        if(partner->role=="Lover" && age>18 && partner->relationship>70)
            std::cout<<"3.Propose marriage\n";
        if(partner->role=="Spouse") std::cout<<"4.Try for child\n";
        std::cout<<"5.Break up/Divorce\n0.Back\n";
        int c;std::cin>>c;
        if(died)return;
        switch(c) {
            case 1:
                partner->relationship+=getRandomNumber(5,15);
                happiness+=5;
                addLifeEvent(std::make_shared<GenericEvent>(age,"Spent time with "+partner->name));
                std::cout<<"\nRelationship improved.\n";
                break;
            case 2:
                if(wealth>=200) {
                    wealth-=200;
                    partner->relationship+=getRandomNumber(10,25);
                    addLifeEvent(std::make_shared<GenericEvent>(age,"Gifted "+partner->name));
                    std::cout<<"\nGift improved relationship.\n";
                } else std::cout<<"\nNot enough money.\n";
                break;
            case 3:
                if(partner->role=="Lover" && age>18 && partner->relationship>70) {
                    {
                        int chance=partner->relationship+(looks/2);
                        if(inventory.hasRing) chance+=20;
                        if(getRandomNumber(1,100)<chance) {
                            partner->role="Spouse";
                            married=true;
                            addLifeEvent(std::make_shared<GenericEvent>(age,"Married "+partner->name));
                            std::cout<<"\nMarried!\n";
                        } else {
                            partner->relationship-=20;
                            std::cout<<"\nRejected.\n";
                        }
                    }
                } else std::cout<<"\nCannot propose now.\n";
                break;
            case 4:
                if(partner->role=="Spouse") {
                    if(promptYesNo("Try for a baby?")) {
                        int babyChance=50;
                        if(getRandomNumber(1,100)<babyChance) {
                            std::string childName=getRandomName();
                            family.push_back(FamilyMember(childName,"Child",50,0,0,getRandomNumber(30,70),false));
                            addLifeEvent(std::make_shared<GenericEvent>(age,"A child was born: "+childName));
                            std::cout<<"\nYou had a child named "<<childName<<"!\n";
                        } else std::cout<<"\nNo pregnancy.\n";
                    }
                } else std::cout<<"\nMust be married.\n";
                break;
            case 5:
                if(partner->role=="Spouse") {
                    addLifeEvent(std::make_shared<GenericEvent>(age,"Divorced "+partner->name));
                    married=false;
                } else {
                    addLifeEvent(std::make_shared<GenericEvent>(age,"Broke up with "+partner->name));
                }
                for(auto it=family.begin();it!=family.end();) {
                    if(&(*it)==partner) it=family.erase(it); else ++it;
                }
                std::cout<<"\nEnded the relationship.\n";
                break;
            default: break;
        }
    } else {
        // single
        std::cout<<"You are single.\n";
        std::cout<<"1.Find date normally\n";
        if(age>=18 && wealth>=100) std::cout<<"2.Use dating app($100)\n";
        std::cout<<"0.Back\n";
        int c;std::cin>>c;
        if(died)return;
        if(c==1) {
            int chance=(looks/2 + happiness/2);
            if(getRandomNumber(1,100)<chance) {
                std::string loverName=getRandomName();
                family.push_back(FamilyMember(loverName,"Lover",getRandomNumber(60,80),age,0,50,false));
                addLifeEvent(std::make_shared<GenericEvent>(age,"Met "+loverName+" started dating."));
                std::cout<<"\nYou are now dating "<<loverName<<".\n";
            } else std::cout<<"\nNo match found.\n";
        } else if(c==2 && age>=18 && wealth>=100) {
            useDatingApp();
        }
    }
}

void Character::familyMenu() {
    if(died)return;
    std::cout<<"\n--- Family Menu ---\n";
    if(family.empty()) {
        std::cout<<"No family.\n";
        return;
    }
    for(size_t i=0;i<family.size();i++) {
        std::cout<<i+1<<". "<<family[i].role<<" "<<family[i].name<<" Age:"<<family[i].age<<" Rel:"<<family[i].relationship<<" $"<<family[i].wealth<<" Gen:"<<family[i].generosity<<(family[i].divorced?"(Divorced)":"")<<"\n";
    }
    std::cout<<"\nChoose # or 0 back:\n";
    int c;std::cin>>c;
    if(died)return;
    if(c<=0||(size_t)c>family.size())return;
    FamilyMember &f=family[c-1];
    std::cout<<"\n1.Spend time\n2.Argue\n3.Give gift\n4.Request money\n5.Activity\n0.Back\n";
    int a;std::cin>>a;
    if(died)return;
    switch(a) {
        case 1:
            f.relationship+=getRandomNumber(5,15);
            happiness+=5;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Spent time with "+f.name));
            std::cout<<"\nRel improved.\n";
            break;
        case 2:
            f.relationship-=getRandomNumber(5,15);
            happiness-=5;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Argued with "+f.name));
            std::cout<<"\nRel worsened.\n";
            break;
        case 3:
            if(wealth>=200) {
                wealth-=200;
                f.relationship+=getRandomNumber(10,25);
                addLifeEvent(std::make_shared<GenericEvent>(age,"Gave gift to "+f.name));
                std::cout<<"\nGift improved relationship.\n";
            } else std::cout<<"\nNot enough money.\n";
            break;
        case 4:
            if((f.role=="Parent"||f.role=="Sibling") && f.relationship>60 && f.generosity>50 && f.wealth>100) {
                {
                    int amt=getRandomNumber(100,std::min(f.wealth/10,1000));
                    f.wealth-=amt;
                    wealth+=amt;
                    addLifeEvent(std::make_shared<GenericEvent>(age,f.name+" gave you $"+std::to_string(amt)));
                    std::cout<<"\n"<<f.name<<" gave you $"<<amt<<"\n";
                }
            } else std::cout<<"\nThey refused.\n";
            break;
        case 5: {
            std::string act;
            int pick=getRandomNumber(1,3);
            if(pick==1) act="fishing";
            else if(pick==2) act="shopping";
            else act="dinner";
            f.relationship+=getRandomNumber(5,10);
            happiness+=5;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Went "+act+" with "+f.name));
            std::cout<<"\nActivity improved relationship & happiness.\n";
        }break;
        default:break;
    }
}

void Character::friendsMenu() {
    if(died)return;
    std::cout<<"\n--- Friends Menu ---\n";
    if(friends.empty()) {
        std::cout<<"No friends.\n";
        return;
    }
    for(size_t i=0;i<friends.size();i++) {
        std::cout<<i+1<<". "<<friends[i].name<<" Age:"<<friends[i].age<<" Rel:"<<friends[i].relationship<<"\n";
    }
    std::cout<<"\nChoose # or 0 back:\n";
    int c;std::cin>>c;
    if(died)return;
    if(c<=0||c>(int)friends.size())return;
    FriendMember &fr=friends[c-1];
    std::cout<<"\n1.Hang out\n2.Argue\n3.Give gift\n4.Party\n0.Back\n";
    int a;std::cin>>a;
    if(died)return;
    switch(a) {
        case 1:
            fr.relationship+=getRandomNumber(5,15);
            happiness+=5;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Hung out with "+fr.name));
            std::cout<<"\nFriend rel improved.\n";
            break;
        case 2:
            fr.relationship-=getRandomNumber(5,15);
            happiness-=5;
            addLifeEvent(std::make_shared<GenericEvent>(age,"Argued with "+fr.name));
            std::cout<<"\nRel worsened.\n";
            break;
        case 3:
            if(wealth>=100) {
                wealth-=100;
                fr.relationship+=getRandomNumber(10,30);
                addLifeEvent(std::make_shared<GenericEvent>(age,"Gifted "+fr.name));
                std::cout<<"\nGift improved relationship.\n";
            } else std::cout<<"\nNot enough money.\n";
            break;
        case 4:
            if(wealth>=200) {
                wealth-=200;
                fr.relationship+=20;
                happiness+=15;
                addLifeEvent(std::make_shared<GenericEvent>(age,"Partied with "+fr.name));
                std::cout<<"\nParty improved relationship & happiness.\n";
            } else std::cout<<"\nNot enough money.\n";
            break;
        default:break;
    }
}

Character::Character(const std::string &n)
: name(n), died(false), inheritedLife(false), lastFriendRequestYear(0), lastLoverRequestYear(0)
{
    srand((unsigned)time(0));
    age=0;
    health=getRandomNumber(50,100);
    happiness=getRandomNumber(50,100);
    intelligence=getRandomNumber(50,100);
    looks=getRandomNumber(50,100);
    wealth=0;
    education=clampStat(intelligence + getRandomNumber(-10,10));
    career="Unemployed";
    hasChosenCareer=false;married=false;hasDriversLicense=false;tookDriversTest=false;
    inCollege=false;collegeMajor="";collegeYearsNeeded=0;collegeYearsCompleted=0;
    fired=false;baseSalary=0;jobStress=30;bossRelationship=50;criminalNotoriety=0;
    deathReason="";

    familyInitialization();
    addLifeEvent(std::make_shared<GenericEvent>(age,"You were born."));
    addLifeEvent(std::make_shared<GenericEvent>(age,"Initial stats:\nHealth="+std::to_string(health)+
                                                " Hap="+std::to_string(happiness)+
                                                " Int="+std::to_string(intelligence)+
                                                " Looks="+std::to_string(looks)+
                                                " $"+std::to_string(wealth)+
                                                " Edu="+std::to_string(education)+" (Edu=academic)"));
}

bool Character::isAlive() const {
    return (health>0 && happiness>0 && !died);
}

void Character::displayStats() const {
    std::cout<<"\n--- Your Stats ---\n";
    std::cout<<"Name: "<<name<<" | Age: "<<age<<"\n";
    std::cout<<"Health: "<<health<<" | Happiness: "<<happiness<<"\n";
    std::cout<<"Intelligence: "<<intelligence<<" | Looks: "<<looks<<"\n";
    std::cout<<"Wealth: $"<<wealth<<" | Education: "<<education<<" (0-100)\n";
    std::cout<<"Career: "<<career<<(fired?" (Fired)":"")<<"\n";
    std::cout<<"Married: "<<(married?"Yes":"No")<<"\n";
    std::cout<<"Driver's License: "<<(hasDriversLicense?"Yes":"No")<<"\n";
    if(inCollege) std::cout<<"In College: "<<collegeMajor<<" ("<<collegeYearsCompleted<<"/"<<collegeYearsNeeded<<")\n";
    if(inventory.hasRing) std::cout<<"Ring: Yes\n";
    if(inventory.hasCar) std::cout<<"Car: "<<inventory.carBrand<<" (Cond:"<<inventory.carCondition<<")\n";
    if(inventory.hasHouse) {
        std::cout<<"House: ";
        if(inventory.houseType==HouseType::Apartment) std::cout<<"Apartment\n";
        else if(inventory.houseType==HouseType::SuburbanHome) std::cout<<"Suburban Home\n";
        else if(inventory.houseType==HouseType::Mansion) std::cout<<"Mansion\n";
    }
    std::cout<<"------------------\n";
}

void Character::displayLifeEvents() const {
    std::cout<<"\n--- Life Events ---\n";
    for(auto &e: lifeEvents) {
        std::cout<<"Age "<<e->getAge()<<": "<<e->getDescription()<<"\n";
    }
    std::cout<<"-------------------\n";
}

void Character::ageOneYearAction() {
    if(!isAlive()) {
        if(!deathReason.empty()) {
            std::cout<<"\nYou are deceased. Reason: "<<deathReason<<"\nGame Over.\n";
        } else {
            std::cout<<"\nYou are deceased. Game Over.\n";
        }
        return;
    }

    age++;
    health=clampStat(health - getRandomNumber(1,3));
    happiness=clampStat(happiness - getRandomNumber(0,2));
    intelligence=clampStat(intelligence + getRandomNumber(0,1));

    if(age==18 && !hasChosenCareer) startCollege();

    addLifeEvent(std::make_shared<GenericEvent>(age,"You grew older by one year."));
    tryDriversLicense();
    yearlyEvents();

    if(died) {
        std::cout<<"\nYou died at age "<<age<<". Reason: "<<deathReason<<"\nGame Over.\n";
    }
}

void Character::showMainMenu() {
    char ch;
    do {
        if(died) {
            break;
        }

        std::cout<<"\n=== MAIN MENU ===\n";
        std::cout<<"1. Age One Year\n";
        std::cout<<"2. View Stats\n";
        std::cout<<"3. Job\n";
        std::cout<<"4. Love\n";
        std::cout<<"5. Family\n";
        std::cout<<"6. Friends\n";
        std::cout<<"7. Possessions\n";
        if(age<18) std::cout<<"8. School\n";
        std::cout<<"9. Health & Happiness\n";
        std::cout<<"A. View Life Events\n";
        std::cout<<"0. Quit\n";
        std::cout<<"Choose: ";
        std::cin>>ch;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            ch='X';
        }

        if(died) break;

        switch(ch) {
            case '1': ageOneYearAction(); break;
            case '2': displayStats(); break;
            case '3': jobMenu(); break;
            case '4': loveMenu(); break;
            case '5': familyMenu(); break;
            case '6': friendsMenu(); break;
            case '7': possessionsMenu(); break;
            case '8': if(age<18) schoolActionsMenu(); else std::cout<<"\nNot in school.\n";break;
            case '9': healthHappinessMenu(); break;
            case 'A':
            case 'a': displayLifeEvents(); break;
            case '0':
                std::cout<<"\nThanks for playing!\n";
                break;
            default:
                std::cout<<"\nInvalid choice.\n";
        }

        if(died) {
            std::cout<<"\nYou died at age "<<age<<". Reason: "<<deathReason<<"\nGame Over.\n";
            break;
        }

        if(ch=='0') break;

    } while(true);
}
