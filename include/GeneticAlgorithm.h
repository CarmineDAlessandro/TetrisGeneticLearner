#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <Individual.h>
#include <Game.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BONUS_PER_LINE 200

struct Log {
    float bestFitness;
    int bestScore;
    float worstFitness;
    float averageFitness;
    float standardDeviation;
};

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm(int populationSize,
                         int mutationProbability,
                         int crossoverProbability,
                         int tournamentSize,
                         int numberElites,
                         unsigned int gameSeed,
                         unsigned int algorithmSeed);

        Individual* getBest();
        Log* runGenerations (int n);

    protected:

    private:
        Individual** population;
        Individual** elites;
        Individual** tournament;
        Log *dataLog;

        /*environment variables*/
        int populationSize;
        int mutationProbability;
        int crossoverProbability;
        int tournamentSize;
        int numberElites;

        /*each individual must play the same game*/
        int gameSeed;
        int algorithmSeed;
        Game* game;

        /*this function make an Individual to play the game at machine-speed*/
        /*it is needed as input the index in the array of the population of
            the selected individual*/
        /*the return value is the fitness associated to that game*/
        float play(int i);
        /*this function calculate the fitness of an instance of the game*/
        float calculateFitness ();
        float evaluatePosition (char** map);

        /*this function makes a tournament among the population selecting the individuals*/
        /*it returns the array after the tournament*/
        Individual** makeTournament ();

        /*used to sort the array of Individuals*/
        static int individualComparator (const void* a, const void* b);

};

#endif // GENETICALGORITHM_H
