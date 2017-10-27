Artificial Intelligence Project - Homework 2

To compile the project, run cmake.

The main should run a loop through 1000 to 10000 episodes
and execute both the monte-carlo and q-learning algorithm.

The executable name is Homework_2.

Makefile :

#Déclaration du projet

project(IAR-robot-cleaner-2)



#Déclaration de l'exécutable

add_executable(

        Homework_2

        src/action.cpp

        src/dp.cpp

        src/world.cpp

        src/main.cpp

        src/montecarlo.cpp

        src/qlearning.cpp

        src/state.cpp

)
