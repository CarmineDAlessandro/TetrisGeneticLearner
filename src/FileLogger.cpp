#include "FileLogger.h"

FileLogger::FileLogger(char* fileName)
{
    this->fileName = fileName;
    /*create a new file overriding the previous one*/
    fp = fopen (fileName, "w");
    /*Here it is only needed to clean the old file*/
    fclose (fp);
}

void FileLogger::log (Individual** population, int populationSize, int generation) {

    /*data needed to be written*/
    int bestFitness;
    int worstFitness;
    float averageFitness;
    float standardDeviation;

    /*getting the data*/
    bestFitness = population[0]->getFitness();
    worstFitness = population[populationSize-1]->getFitness();

    averageFitness = 0;
    for (int i = 0; i < populationSize; i++) {
        averageFitness += population[i]->getFitness();
    }
    averageFitness /= populationSize;

    standardDeviation = 0;
    for (int i = 0; i < populationSize; i++) {
        standardDeviation += pow(population[i]->getFitness() - averageFitness, 2);
    }
    standardDeviation = sqrt (standardDeviation / populationSize);

    /*appending the data to the file*/
    fp = fopen (fileName, "a");

    fprintf(fp,
            "Generation: %d\n\tBest fitness: %d\n\tWorst fitness: %d\n\tAverage Fitness: %f\n\tStandard Deviation: %f\n",
             generation, bestFitness, worstFitness, averageFitness, standardDeviation);

    fclose(fp);
}

