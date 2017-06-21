#include "GameView.h"

#include <string.h>

#include <windows.h>
#include <GL/glut.h>

Game* GameView::game = NULL;

GameView::GameView(Game* game)
{
    GameView::game = game;
}


/*the main drawing function*/
void GameView::drawGame() {

    /*clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT);

    drawScore();
    drawMap();

    /*draw on the screen*/
    glutSwapBuffers();
}


void GameView::drawScore() {
    char title [] = "TETRIS";
    char score [20] = "Score: ";
    char vscore [10];
    char level [20] = "Level: ";
    char vlevel [10];
    char pause [] = "Game Paused - press any key to start";
    char gameover [] = "GAME OVER";

    int c;

    itoa(game->getScore(), vscore, 10);
    itoa(game->getLevel()+1, vlevel, 10);

    strcat(score, vscore);
    strcat(level, vlevel);

    //the color: WHITE
    glColor3f(1.0f,1.0f,1.0f);

    /*the title*/
    glRasterPos2f(-0.2f, 0.9f);
    c = 0;
    while (title[c] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[c]);
        c++;
    }

    /*the score*/
    glRasterPos2f(-0.9f, 0.8f);
    c = 0;
    while (score[c] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score[c]);
        c++;
    }

    /*the level*/
    glRasterPos2f(-0.9f, 0.7f);
    c = 0;
    while (score[c] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, level[c]);
        c++;
    }

    /*if the game is over*/
    if (!game->isPlaying()) {
        glRasterPos2f(-0.3f, 0.6f);
        c = 0;

        while (gameover[c] != '\0') {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameover[c]);
            c++;
        }
    }
    /*if the game is paused*/
    else if (game->isInPause()) {
        glRasterPos2f(-0.9f, 0.6f);
        c = 0;

        while (pause[c] != '\0') {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pause[c]);
            c++;
        }


    }


}

/*this function draws a cube.
    It needs the element, the absolute position (x,y)
    and the dimension of the cube


    The position means the absolute position
    of the left-upper corner expressed in float
*/
void GameView::drawCube(char el, float x, float y, float dim) {

    /*set the color according to the element to be displayed*/
    switch (el) {
    case 'Z':
    case 'z':
        glColor3f(1.0f, 0.0f, 0.0f);
        break;
    case 'S':
    case 's':
        glColor3f(0.0f, 1.0f, 0.0f);
        break;
    case 'J':
    case 'j':
        glColor3f(0.0f, 0.0f, 1.0f);
        break;
    case 'Q':
    case 'q':
        glColor3f(1.0f, 1.0f, 0.0f);
        break;
    case 'T':
    case 't':
        glColor3f(1.0f, 0.0f, 1.0f);
        break;
    case 'I':
    case 'i':
        glColor3f(0.0f, 1.0f, 1.0f);
        break;
    case 'L':
    case 'l':
        glColor3f(1.0f, 0.5f, 0.0f);
        break;
    case 'X':
    case 'x':
        drawCube(game->getCurrentPiece(), x, y, dim);
        break;
    default :
        glColor3f(1.0f, 1.0f, 1.0f);
        break;
    }

    glTranslatef( x, y ,1);

    glBegin(GL_POLYGON);
        glVertex2f (0, 0);
        glVertex2f (0, dim);
        glVertex2f (dim, dim);
        glVertex2f (dim, 0);
    glEnd();

}

void GameView::drawMap () {
    /*the upper left corner of the map*/
    const float xOrigin = -0.475f;
    const float yOrigin = 0.4f;

    /*the dimension of a cube*/
    const float stdDimension = 0.08f;
    /*I have tried to add also a grid, but the output was too ugly*/
    //const float gridDimension = 0.00325f;

    /*the map to be visualized*/
    char** map = game->getMap();

    for (int i = 0; i < game -> v; i ++) {
        for (int j = 0; j < game -> h; j ++) {

            glPushMatrix();
                drawCube(map[i][j],

                         xOrigin + j * stdDimension,
                         yOrigin - i * stdDimension,
                         stdDimension);
            glPopMatrix();
        }
    }

}


