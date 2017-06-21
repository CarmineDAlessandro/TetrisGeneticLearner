#include "Controller.h"

#include <windows.h>
#include <GL/glut.h>

Individual* Controller::best = NULL;
Game* Controller::game = NULL;
GameView* Controller::view = NULL;
int Controller::tic = 0;
int Controller::chromosomeStep = 0;
bool Controller::simulationRunning = false;

Controller::Controller(Individual* best, GameView* view, Game* game)
{
    this->best = best;
    this->view = view;
    this->game = game;

}

/*this function is almost the same of the GeneticAlgorithm.cpp function play*/
/*the only difference between the 2 is that here the actions are
performed at human speed because of the timer*/
void Controller::nextStep (int value) {

    if (chromosomeStep < CHROMOSOME_SIZE &&
             game->isPlaying()){

        /*if is possible*/
        if (best->genotype[chromosomeStep] &&
            best->phenotype[chromosomeStep] &&
            tic < game->getSpeed()) {

            /*perform the action*/
            switch (best->genotype[chromosomeStep]) {
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
    }

    glutPostRedisplay();
    glutTimerFunc(ticIncrement, nextStep, value);
}

/*wait the user to press a key to start the simulation*/
void Controller::keyboardFunc(unsigned char key, int x, int y) {
    if (!simulationRunning) {
        switch(key) {
        default:
            simulationRunning = true;
            game->unpause();
            glutTimerFunc(ticIncrement, nextStep, GAME_TIMER);
            break;
        }
    }
    if (simulationRunning &&
        !game->isPlaying())
        switch(key) {
        default:
            exit(EXIT_SUCCESS);
            break;
        }

}

void Controller::start (int* argcp, char** argv) {

    glutInit(argcp, argv);

    /*the frame*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,550);
    glutInitWindowPosition(200,100);
    glutCreateWindow("Tetris Learner");

    /*settings*/
    glutDisplayFunc(view->drawGame);
    glutKeyboardFunc(keyboardFunc);

    glutMainLoop();
}
