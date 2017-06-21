#include "Individual.h"

#include <stdlib.h>

Individual::Individual()
{
    for (int i = 0; i < CHROMOSOME_SIZE; i ++) {
        /*W A S D*/
        switch (rand () % 4) {
        case ROTATION: genotype[i] = ROTATION; break;
        case MOVELEFT: genotype[i] = MOVELEFT; break;
        case MOVEDOWN: genotype[i] = MOVEDOWN; break;
        case MOVERIGHT: genotype[i] = MOVERIGHT; break;
            default : break;
        }

        phenotype[i] = rand()%2;
    }
    fitness = 0;
}



Individual* Individual::clone() {
    Individual* newEl = new Individual ();

    for (int i = 0; i < CHROMOSOME_SIZE; i ++) {
        newEl->genotype[i] = genotype[i];

        newEl->phenotype[i] = phenotype[i];
    }

    newEl->setFitness(fitness);

    return newEl;
}

void Individual::mutate(int prob) {
    /*used for the mutation of the phenotype*/
    int prev;

    for (int i = 0; i < CHROMOSOME_SIZE; i ++) {
        if(prob > rand()%10001) {
            prev = genotype[i];
            do {
                switch (rand () % 4) {
                case ROTATION: genotype[i] = ROTATION; break;
                case MOVELEFT: genotype[i] = MOVELEFT; break;
                case MOVEDOWN: genotype[i] = MOVEDOWN; break;
                case MOVERIGHT: genotype[i] = MOVERIGHT; break;
                    default : break;
                }
            } while (prev == genotype[i]);
        }

        if(prob > rand()%10001) {
            phenotype[i] = !phenotype[i];
        }
    }
}

void Individual::crossover(Individual* partner, int prob) {
    /*if "true" the chromosome are making crossover*/
    bool isCrossovering;
    /*the temporary variables used for the exchange */
    Movement temp;
    bool temp1;

    /*each chromosome is considered independently*/

    isCrossovering = false;

    for (int j = 0; j < CHROMOSOME_SIZE; j ++) {

        if (prob > rand()%10001)
            isCrossovering = !isCrossovering;

        if (isCrossovering){
            temp = genotype[j];
            genotype[j] = partner->genotype[j];
            genotype[j] = temp;
        }

    }



    isCrossovering = false;
    for (int i = 0; i < CHROMOSOME_SIZE; i ++) {

        if (prob > rand()%10001)
                isCrossovering = !isCrossovering;

        if (isCrossovering) {
            temp1 = phenotype[i];
            phenotype[i] = partner->phenotype[i];
            partner->phenotype[i] = temp1;
        }

    }


}

float Individual::getFitness() {
    return fitness;
}

void Individual::setFitness (float fitness) {
    this->fitness = fitness;
}

int Individual::getScore() {
    return score;
}

void Individual::setScore(int score) {
    this->score = score;;
}
