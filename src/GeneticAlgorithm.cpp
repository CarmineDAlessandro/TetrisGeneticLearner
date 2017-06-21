#include "GeneticAlgorithm.h"


GeneticAlgorithm::GeneticAlgorithm(int populationSize,
                                   int mutationProbability,
                                   int crossoverProbability,
                                   int tournamentSize,
                                   int numberElites,
                                   unsigned int gameSeed,
                                   unsigned int algorithmSeed)
{
    this->populationSize = populationSize;
    this->mutationProbability = mutationProbability;
    this->crossoverProbability = crossoverProbability;
    this->tournamentSize = tournamentSize;
    this->numberElites = numberElites;
    this->gameSeed = gameSeed;
    this->algorithmSeed = algorithmSeed;

    /*check for the correctness of the constraints about the size of the population*/

    if (populationSize % 2 != 0) {
        printf("populationSize cannot be ODD. New value %d\n", populationSize+1);
        this->populationSize++;
    }
    if (populationSize < tournamentSize) {
        printf("tournamentSize too big. New value %d\n", populationSize);
        this->tournamentSize = populationSize;
    }
    if (populationSize < numberElites) {
        printf("numberElites too big. New value: %d\n", populationSize);
        this->numberElites = tournamentSize;
    }



    /*initializing the population (randomly)*/
    population = new Individual* [populationSize];
    for (int i = 0; i < populationSize; i ++) {
        /*setting randomly the chromosomes*/
        population[i] = new Individual();

        /*calculating the fitness for each member*/
        /*each individual must play the same game*/
        srand(gameSeed);
        delete game;
        game = new Game();

        /*play the game to calculate the fitness of the individual*/
        population[i]->setFitness(play(i));

    }
    srand(algorithmSeed);

    elites = new Individual* [numberElites];
    tournament = new Individual* [tournamentSize];

    this->game = game;

}

Individual* GeneticAlgorithm::getBest() {
    qsort (population, populationSize, sizeof(Individual*), individualComparator);
    return population[0];
}

Log* GeneticAlgorithm::runGenerations(int n) {
    /*initialize data log array*/
    dataLog = new Log[n];

    /*start running generations*/
    while (n > 0) {

        /*save elites*/
        for (int i = 0; i < numberElites; i ++) {
            elites[i] = population[i]->clone();
        }

        /*make tournament*/
        population = makeTournament();

        /*mutate*/
        for (int i = 0; i < populationSize; i ++) {
            population[i]->mutate(mutationProbability);
        }

        /*crossover*/
        for (int i = 0; i < populationSize; i = i + 2) {
            population[i]->crossover(population[i+1], crossoverProbability);
        }

        /*save the current element of the sequence*/
        algorithmSeed = rand();


        /*calculate the fitness for each member*/
        for (int i = 0; i < populationSize; i ++) {
            /*each individual must play the same game*/
            srand(gameSeed);
            delete game;
            game = new Game();

            /*execute movements*/
            /*set the fitness to the individual*/
            population[i]->setFitness(play(i));
            population[i]->setScore(game->getScore());
        }


        /*set the sequence again to the ending point saved before the for cycle*/
        srand(algorithmSeed);

        /*sort*/
        qsort (population, populationSize, sizeof(Individual*), individualComparator);

        /*add the elites to the population deleting the worst elements*/
        for (int i = 0; i < numberElites; i ++) {
            delete population[populationSize-i-1];
            /*add the elites to the population*/
            population[populationSize-i-1] = elites[i];
        }

        /*sort again*/
        qsort (population, populationSize, sizeof(Individual*), individualComparator);


        n--;

        /*logging the data of the generation*/
        dataLog[n].bestFitness = population[0]->getFitness();
        dataLog[n].bestScore = population[0]->getScore();
        dataLog[n].worstFitness = population[populationSize-1]->getFitness();
        dataLog[n].averageFitness = 0;


        for (int i = 0; i < populationSize; i ++) {
            dataLog[n].averageFitness += population[i]->getFitness();
        }
        dataLog[n].averageFitness /= populationSize;


        /*standard deviation*/
        dataLog[n].standardDeviation = 0;
        for (int i = 0; i < populationSize; i ++) {
            dataLog[n].standardDeviation = pow( dataLog[n].averageFitness - population[i]->getFitness(), 2);
        }
        dataLog[n].standardDeviation /= (populationSize-1);
        sqrt (dataLog[n].standardDeviation);
    }

    return dataLog;
}

float GeneticAlgorithm::play (int i) {
    int chromosomeStep = 0;
    /*the next two variables are needed to synchronize the
    machine to the game. ticIncrement set to 25 means that the machine
    makes a movement each 25 milliseconds, tic is the time in
    milliseconds passed from the last call to stepBottom() of the game
    if tic == game->speed a game->stepBottom() must be performed and
    the tic variable is set again to 0
    */
    const int ticIncrement = 100;
    int tic = 0;

    /*if the score increase after the movement a piece
        is landed and there is need to evaluate the position */
    int scoreBefore, scoreAfter;

    /*saving the score before the action*/
    scoreBefore = game->getScore();

    while (chromosomeStep < CHROMOSOME_SIZE &&
             game->isPlaying()){
        /*if is possible*/
        if (population[i]->genotype[chromosomeStep] &&
            population[i]->phenotype[chromosomeStep] &&
            tic < game->getSpeed()) {

            /*perform the action*/
            switch (population[i]->genotype[chromosomeStep]) {
            /*W*/
            case ROTATION:
                game->rotatePiece();
                break;
            /*A*/
            case MOVELEFT:
                game->moveLeft();
                break;
            /*S*/
            case MOVEDOWN:
                game->stepBottom();
                break;
            /*D*/
            case MOVERIGHT:
                game->moveRight();
                break;
            /**/
            default:
                break;
            }


        }

        /*if it is time to move the piece one step down, move it*/
        if (tic >= game->getSpeed()) {
            game->stepBottom();
            tic -= game->getSpeed();
        }


        /*increase the time of the game*/
        tic += ticIncrement;

        /*increase the gene considered*/
        chromosomeStep++;

        scoreAfter = game->getScore();

        if (scoreAfter > scoreBefore)
            /*something = */evaluatePosition(game->getMap());

    }

    /*finally calculate the fitness of the solution*/
    return calculateFitness();
}


float GeneticAlgorithm::calculateFitness() {

    /*the score*/
    return  game->getScore();

}

float GeneticAlgorithm::evaluatePosition(char ** map) {
    float fitness = 0;

    return fitness;
}


Individual** GeneticAlgorithm::makeTournament() {
    Individual **newPopulation = new Individual* [populationSize];
    int winner;

    int random;
    int numberParticipants;

    int j;

    for (int i = 0; i < populationSize; i ++) {
        numberParticipants = 0;

        /*select participants*/
        while (numberParticipants < tournamentSize) {

            /*extract a possible participant*/
            random = rand () % populationSize;

            /*check if it is not already contained in the tournament array*/
            /*if it is, extract again a possible participant while you do not
            find a valid one*/
            j = 0;
            while (j < numberParticipants) {
                if (tournament[j] == population[random]) {
                    random = rand () % populationSize;
                    j = -1;
                }
                j++;
            }

            /*add it to the tournament*/
            tournament[numberParticipants] = population[random];
            numberParticipants++;
        }

        /*sort*/
        qsort (tournament, tournamentSize, sizeof(Individual*),individualComparator);

        winner = 0;
        /*chose the winner*/
        while (winner < tournamentSize-1 &&
               rand()%100 >= 50) {
                winner++;
               }

        /*copy the winner into the new population*/
        newPopulation[i] = tournament[winner]->clone();
    }

    /*delete the old population*/
    for (int i = 0; i < populationSize; i ++) {
        delete population[i];

    }
    delete []population;


    return newPopulation;
}


/*this function orders an array of individuals in a decreasing order of fitness*/
int GeneticAlgorithm::individualComparator (const void* a, const void* b) {
    if ((*(Individual**)b)->getFitness() > (*(Individual**)a)->getFitness())
        return 1;

    if ((*(Individual**)a)->getFitness() > (*(Individual**)b)->getFitness())
        return -1;


    return 0;

}
