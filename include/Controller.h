#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Individual.h>
#include <GameView.h>

#define GAME_TIMER 1

class Controller
{
    public:
        Controller(Individual* best, GameView* view, Game* game);
        static void start (int *argcp, char** argv);

    protected:

    private:
        static bool simulationRunning;

        static Individual* best;
        static GameView* view;
        static Game* game;

        static int chromosomeStep;
        static int tic;
        static const int ticIncrement = 100;

        static void keyboardFunc (unsigned char key, int x, int y);
        static void nextStep (int value);
};

#endif // CONTROLLER_H
