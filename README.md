Enhanced Life Simulation
A text-based life simulation game where you start as a newborn character and guide your life through various decisions—managing health, happiness, education, career, relationships, possessions, and more—until you pass away. Inspired by life simulator games, this project provides a simple but expandable framework for a console-based interactive experience.

Features
Aging System: Advance time year-by-year and watch your character grow from infancy into adulthood.
Character Stats: Manage health, happiness, intelligence, looks, wealth, and education.
Relationships: Interact with parents, siblings, romantic partners, and friends. Improve or worsen relationships through various actions.
Careers & Education: Attend college, choose a major, graduate, and pursue a career. Legit jobs come with income and stress, while criminal careers offer risky but lucrative opportunities.
Possessions: Buy and sell items such as houses, cars, and rings. Owning property can boost your happiness.
Random Events: Encounter random life events like parental divorces, new siblings, friend requests, and unexpected challenges.
Choices & Consequences: Decisions made each year can impact your character’s stats, relationships, and financial situation, ultimately affecting how long they live and how fulfilling their life becomes.
Installation & Usage

Prerequisites
A C++ compiler (such as g++ or clang++) that supports C++11 or higher.
A terminal or command prompt environment to run the compiled program.
Steps to Build
Clone the repository:
bash
Copy code
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name
Compile the code: Using g++, for example:
bash
Copy code
g++ main.cpp character.cpp lifeevent.cpp familyfriend.cpp utilities.cpp -o life_sim
Make sure all .cpp files are included. If you have a different project structure, adjust the compilation command accordingly.

to use the program:
bash
Copy code
./life_sim

Gameplay
Upon starting, you’ll be prompted to enter your character’s name.
The main menu allows you to:
Age your character by one year.
View your character’s stats.
Manage job, love life, family, friends, possessions, school activities, and well-being.
Input the corresponding number or letter to choose an action.
Continue making choices year after year until your character passes away.
Controls
All input is done via keyboard. Follow on-screen prompts.
Use y or n for yes/no questions.
Press the corresponding number keys or letters to navigate menus.

Contributions are welcome! If you’d like to help improve this simulation:

