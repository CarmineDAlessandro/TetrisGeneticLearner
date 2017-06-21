
#include <GeneticAlgorithm.h>
#include <Controller.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char* argv[]) {

    /*variables*/
    int populationSize;
    int mutationProbability;
    int crossoverProbability;
    int tournamentSize;
    int numberElites;
    int numberGenerations;
    unsigned int seed;

    /*monitoring the execution time*/
    int start_s = 0, stop_s = 0;
    int seconds = 0, minutes = 0, hours = 0;

    /*data log*/
    Log* dataLog;
    char nameLog[] = "..\\..\\geneticLog.txt";
    char nameBestLog[] = "..\\..\\bestLog.dat";
    FILE *fileLog, *bestLog;

    /*data input*/
    char nameInput[] = "..\\..\\learnerInput.txt";
    FILE* fileInput;

    if ((fileInput = fopen (nameInput,"r"))) {

        /*reading the input from input file*/
        fscanf(fileInput,"populationSize: %d\n", &populationSize);
        fscanf(fileInput,"mutationProbability: %d\n", &mutationProbability);
        fscanf(fileInput,"crossoverProbability: %d\n", &crossoverProbability);
        fscanf(fileInput,"tournamentSize: %d\n", &tournamentSize);
        fscanf(fileInput,"numberElites: %d\n", &numberElites);
        fscanf(fileInput,"numberGenerations: %d\n", &numberGenerations);
        fscanf(fileInput,"seed: %d\n", &seed);

        if (seed == 0)
            seed = (unsigned int) time (NULL);

        printf("Setting input data from file:\n");
    } else {

        /*auto setting the variables*/
        populationSize = 50;
        mutationProbability = 5;
        crossoverProbability = 4000;
        tournamentSize = 8;
        numberElites = 2;
        numberGenerations = 50;
        seed = (unsigned int) time (NULL);
        printf("Auto generating input data:\n");
    }

    /*show the input data before the run*/
    printf("populationSize: %d\n",populationSize);
    printf("mutationProbability: %d/10000\n",mutationProbability);
    printf("crossoverProbability: %d/10000\n",crossoverProbability);
    printf("tournamentSize: %d\n",tournamentSize);
    printf("numberElites: %d\n",numberElites);
    printf("numberGenerations: %d\n",numberGenerations);
    printf("seed: %d\n",seed);
    printf("\n");


    /*pseudo casual seeds*/
    srand(seed);
    unsigned int gameSeed = (unsigned int) rand();
    unsigned int algorithmSeed = (unsigned int) rand();

    GeneticAlgorithm* genetic =
            new GeneticAlgorithm (populationSize, mutationProbability,
                                crossoverProbability, tournamentSize,
                                numberElites, gameSeed, algorithmSeed );


    printf("Starting the run of the genetic algorithm\n\n");

    start_s = clock();
    /*run the generation of the algorithm*/
    dataLog = genetic->runGenerations(numberGenerations);

    stop_s = clock();

    /*get the best individual*/
    Individual* best = genetic->getBest();



    printf("Logging Data\n");
    /*saving the best individual*/
    bestLog = fopen (nameBestLog, "w");
    for (int i = 0; i < CHROMOSOME_SIZE; i ++) {
        fprintf(bestLog,"%d %d", best->genotype[i],
                                          best->phenotype[i]);
    }


    fclose(bestLog);

    /*saving the data log*/

    fileLog = fopen (nameLog, "w");

    /*logging the input data*/
    fprintf(fileLog,"Input data:\n");
    fprintf(fileLog,"populationSize: %d\n",populationSize);
    fprintf(fileLog,"mutationProbability: %d/10000\n",mutationProbability);
    fprintf(fileLog,"crossoverProbability: %d/10000\n",crossoverProbability);
    fprintf(fileLog,"tournamentSize: %d\n",tournamentSize);
    fprintf(fileLog,"numberElites: %d\n",numberElites);
    fprintf(fileLog,"numberGenerations: %d\n",numberGenerations);
    fprintf(fileLog,"gameSeed: %d\n\n", gameSeed);

    seconds = (int) (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
    seconds /= 1000;

    /*calculating minutes and hours*/
    minutes = seconds / 60;
    seconds = seconds % 60;

    hours = minutes / 60;
    minutes = minutes % 60;

    fprintf(fileLog,"Execution time\n%d:%d.%d\n\n", hours, minutes, seconds);

    /*logging the result of the algorithm*/
    for (int i = numberGenerations-1; i >= 0; i --) {
        fprintf(fileLog,"generation %3d - ", numberGenerations - i);
        fprintf(fileLog,"best: %3f (%d) - ", dataLog[i].bestFitness, dataLog[i].bestScore);
        fprintf(fileLog,"worst: %3f - ", dataLog[i].worstFitness);
        fprintf(fileLog,"avg fitness: %f - ", dataLog[i].averageFitness);
        fprintf(fileLog,"std dev: %f\n", dataLog[i].standardDeviation);
    }

    fclose(fileLog);



    printf("Visualizing the best solution\n");
    printf("Best fitness: %f\n\n", best->getFitness());
    /*make the best game to start in an human speed*/
    srand (gameSeed);
    Game* game = new Game ();
    GameView* view = new GameView(game);
    Controller* controller = new Controller (best, view, game);

    controller->start(&argc, argv);


    return EXIT_SUCCESS;
}
