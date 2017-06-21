#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

enum Movement {ROTATION = 0, MOVELEFT = 1, MOVEDOWN = 2, MOVERIGHT = 3};

#define CHROMOSOME_SIZE 10000

class Individual
{
    public:
        /*generate random genotype and phenotype*/
        Individual();
        Individual* clone();


        /*the probability is expressed in values/10'000
        examples

            probability = 9000 means that you have 9000/10000 chance, so 90%
            probability = 1 means that you have 1/10000 chance, so 0.01%

            increasing the probability by 1 unit means that you are
            increasing the probability of the event of 0.01%

        */
        void mutate(int probability);
        void crossover(Individual* partner, int probability);

        /*getters and setters*/
        void setFitness (float fitness);
        float getFitness ();
        void setScore (int score);
        int getScore();

        /*
        there is 1 chromosome that memorize in each cell
        one of the possible inputs from the used
        */
        Movement genotype [CHROMOSOME_SIZE];

        /*
        the value of each cell of the phenotype expresses
        the possibility or not that the correspondent cell
        of the genotype is expressed

        */
        bool phenotype [CHROMOSOME_SIZE];

    protected:

    private:
        float fitness;
        int score;

};

#endif // INDIVIDUAL_H
