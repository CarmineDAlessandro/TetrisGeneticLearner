#include "Game.h"
#include <stdlib.h>
#include <ctype.h>


/*the game*/

/*  CONSTRUCTOR  */

Game::Game()
{
    score = 0;
    level = 0;

    /*initialize the map*/
    map = (char**) malloc (sizeof(char*) * v);
    for (int i = 0; i < v; i ++) {
        map[i] = (char*) malloc (sizeof(char) * h);
        for (int j = 0; j < h; j ++) {
            map[i][j] = ' ';
        }
    }

    /*generate the first piece*/
    generatePiece();

    isPaused = true;
    playing = true;


    /*initialize the variables needed for the genetic algorithm*/
    linesRemoved = 0;
}


/*  PUBLIC METHODS  */


/*getters*/
int Game::getScore() {
    return score;
}

int Game::getLevel() {
    return level;
}

const float Game::getSpeed() {
    return speed[level][0];
}

char** Game::getMap () {
    return map;
}

char Game::getCurrentPiece() {
    return currentPiece;
}




bool Game::isInPause() {
    return isPaused;
}

bool Game::isPlaying () {
    return playing;
}





/*control functions*/
void Game::moveLeft() {
    bool isPossible = true;

    /*check if it is possible to move left*/
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {
            if (isupper(map[i][j])) {
                if (j <= 0 || (
                    map[i][j-1] != ' ' &&
                    islower(map[i][j-1])
                    )) {
                        isPossible = false;
                }
            }
        }
    }

    if (isPossible) {
        /*shift the piece to left*/
        for (int j = 0; j < h; j ++) {
            for (int i = 0; i < v; i ++) {
                if (isupper(map[i][j])) {
                    map[i][j-1] = map[i][j];
                    map[i][j] = ' ';
                }
            }
        }

    }
}
void Game::moveRight() {
    bool isPossible = true;

    /*check if it is possible to move left*/
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {
            if (isupper(map[i][j])) {
                if (j >= h-1 || (
                    map[i][j+1] != ' ' &&
                    islower(map[i][j+1])
                    )) {
                        isPossible = false;
                }
            }
        }
    }

    if (isPossible) {
        /*shift the piece to left*/
        for (int j = h-1; j >= 0; j --) {
            for (int i = 0; i < v; i ++) {
                if (isupper(map[i][j])) {
                    map[i][j+1] = map[i][j];
                    map[i][j] = ' ';
                }
            }
        }

    }
}


void Game::stepBottom() {
    isLanded = false;


    /*check if the piece is landed*/
    /*if yes, put the isLanded flag*/
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {

            /*detect for a flying piece*/
            if (isupper(map[i][j])) {
                /*if the cell below the current one exists and is not empty or occupied by the same element of the current*/
                if (i >= v-1 || (
                    map[i+1][j] != ' ' &&
                    islower(map[i+1][j])
                    )) {
                    /*the piece is landed*/
                    isLanded = true;
                }

            }


        }
    }   /*end check-landing for*/

    if (isLanded)
        playing = detectedLanding();

    else {
        /*shift all the flying pieces down by 1*/
        for (int i = v-1; i >= 0; i --) {
            for (int j = h-1; j >= 0; j --) {
                if (isupper(map[i][j])) {
                    map[i+1][j] = map[i][j];
                    map[i][j] = ' ';
                }
            }
        }
        /*continue playing*/
        playing = true;
    }
}

void Game::allBottom(){
    isLanded = false;

    /*check for the landing*/
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {

            /*detect for a flying piece*/
            if (isupper(map[i][j])) {
                /*if the cell below the current one exists and is not empty or occupied by the same element of the current*/
                if (i >= v-1 || (
                    map[i+1][j] != ' ' &&
                    islower(map[i+1][j])
                    )) {
                    /*the piece is landed*/
                    isLanded = true;
                }

            }


        }
    }   /*end check-landing for*/


    while (!isLanded) {

        /*shift all the flying pieces down by 1*/
        for (int i = v-1; i >= 0; i --) {
            for (int j = h-1; j >= 0; j --) {
                if (isupper(map[i][j])) {
                    map[i+1][j] = map[i][j];
                    map[i][j] = ' ';
                }
            }
        }

        /*check for the landing*/
        for (int i = 0; i < v; i ++) {
            for (int j = 0; j < h; j ++) {

                /*detect for a flying piece*/
                if (isupper(map[i][j])) {
                    /*if the cell below the current one exists and is not empty or occupied by the same element of the current*/
                    if (i >= v-1 || (
                        map[i+1][j] != ' ' &&
                        islower(map[i+1][j])
                        )) {
                        /*the piece is landed*/
                        isLanded = true;
                    }

                }


            }
        }   /*end check-landing for*/

    }

    /*piece landed*/
    playing = detectedLanding();
}

/*rotation performed in clockwise*/
void Game::rotatePiece () {

    /*the square does not rotate*/
    if (currentPiece == 'Q')
        return;

    int xRot = 0, yRot = 0;
    bool isPossible = true;

    /*detect the center of the rotation*/
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {
            if (map[i][j] == 'X') {
                xRot = i;
                yRot = j;
            }

        }

    }

    /*
    There are 4 parts of the field individuated
    by the 'X' in the middle of the piece

              |
           1  |    2
              T
        -----TXT-------
              |
              |
           3  |    4
              |
              |

    To perform a rotation the program have to
    rotate 1 into 2, 2 into 4, 4 into 3, 3 into 4

    The pieces

          I
     T    I   ZZ    SS   XLL   JJX
    TXT   X    XZ  SX    L       J
          I
    */

    /*for each piece the program have to check if it is possible to move*/
    /*if it is possible to move, the program have to move it*/

    /*It is possible to do it don't making a switch, but considering an other
      piece obtained by the overlap of all the pieces (except Q, of course)*/
    /*We call that piece only P

            P
          P P P
        P P X P P
          P P P
            P

       So it's possible to consider for the rotation only the part of field
       around X marked with the P
    */




    /*this array contains, for each position, the variation from xRot and yRot
        of the beginning point of the rotation and the ending point*/
    const int rotationArray [12][4] = {
        /*xFrom, yFrom, xTo, yTo*/
        { -2, 0, 0, 2},
        { -1, -1, -1, 1},
        { -1, 0, 0, 1},
        { -1, 1, 1, 1},
        { 0, -2, -2, 0},
        { 0, -1, -1, 0},
        { 0, 1, 1, 0},
        { 0, 2, 2, 0},
        { 1, -1, -1, -1},
        { 1, 0, 0, -1},
        { 1, 1, 1, -1},
        { 2, 0, 0, -2}
    };

    int xFrom, yFrom;
    int xTo, yTo;

    for (int i = 0; i < 12; i ++) {
        xFrom = xRot + rotationArray[i][0];
        yFrom = yRot + rotationArray[i][1];
        xTo = xRot + rotationArray[i][2];
        yTo = yRot + rotationArray[i][3];


        if (isPossible)
            /*constrains about the origin cell*/
            if (xFrom >= 0 && xFrom < v &&
                yFrom >= 0 && yFrom < h &&
                isupper(map[xFrom][yFrom]))
                /*constraints about the destination cell*/
                if (xTo < 0 || xTo >= v ||
                    yTo < 0 || yTo >= h ||
                    islower(map[xTo][yTo]))
                    isPossible = false;



    }

    /*used for the rotation*/
    int temp [3][2];
    int parts;

    if (isPossible) {
        /*rotate*/
        parts = 0;
        /*for each cell of the rotation array*/
        for (int i = 0; i < 12; i ++){

            xFrom = xRot + rotationArray[i][0];
            yFrom = yRot + rotationArray[i][1];
            xTo = xRot + rotationArray[i][2];
            yTo = yRot + rotationArray[i][3];

            /*save the destination position and delete the piece*/
            if (xFrom >= 0 && xFrom < v &&
                yFrom >= 0 && yFrom < h &&
                isupper(map[xFrom][yFrom])) {

                temp[parts][0] = xTo;
                temp[parts][1] = yTo;
                /*clean the position*/
                map[xFrom][yFrom] = ' ';
                parts++;
            }

        }

        /*for each piece saved into the temp array*/
        for (int i = 0; i < 3; i ++) {
            /*redraw the piece rotated*/
            map[temp[i][0]][temp[i][1]] = currentPiece;
        }

    }

}

void Game::pause() {
    isPaused = true;
}
void Game::unpause() {
    isPaused = false;
}


/*  PRIVATE METHODS  */


/*the are 2 numbers,
    the first one is the speed
    the second one is the score needed to reach the level


    speed = 1.0f / speed at that level
*/
const float Game::speed [15][2] = {
          {1000.0f, 0},
          { 900.0f, 100},
          { 800.0f, 200},
          { 700.0f, 300},
          { 600.0f, 400},
          { 550.0f, 550},
          { 500.0f, 700},
          { 450.0f, 850},
          { 400.0f, 1000},
          { 350.0f, 1150},
          { 300.0f, 1300},
          { 250.0f, 1450},
          { 200.0f, 1600},
          { 150.0f, 1750},
          { 100.0f, 1900}
        };


void Game::increaseScore(int addend) {
    score += addend;

    if (level < MAX_LEVEL && speed[level+1][1] <= score)
        level ++;
}


int Game::removeLines() {
    int nrLinesRemoved = 0;
    bool lineRemoved;

    for (int i = 0; i < v; i ++) {
        lineRemoved = true;


        /*check if it is possible to remove the line*/
        for (int j = 0; j < h; j ++) {

            if (map[i][j] == ' ')
                lineRemoved = false;
        }


        /*remove the line*/
        if(lineRemoved) {

            /*shift down all the lines before the current one by 1*/
            for (int k = i-1; k >= 0; k --) {
                for (int j = 0; j < h; j ++)
                    map[k+1][j] = map[k][j];
            }

            nrLinesRemoved ++;
        }
    }

    linesRemoved += nrLinesRemoved;
    return nrLinesRemoved * SCORE_PER_LINE;
}

bool Game::generatePiece () {
    bool isPossible = true;

    switch (rand() % 7) {
    case LINE:
        /*generate lIne

            IIII

        */

        /*check for collisions with other pieces*/
        if (
            map[0][3] != ' ' ||
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[0][6] != ' '
            ) {
                isPossible = false;
            }

        map[0][3] = 'I';
        map[0][4] = 'I';
        map[0][5] = 'X';
        map[0][6] = 'I';

        currentPiece = 'I';

        break;
    case SQUARE:
        /*generate sQuare

            QQ
            QQ

        */

        if (
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[1][4] != ' ' ||
            map[1][5] != ' '
            ) {
                isPossible = false;
            }

        map[0][4] = 'Q';
        map[0][5] = 'Q';
        map[1][4] = 'Q';
        map[1][5] = 'Q';

        currentPiece = 'Q';

        break;
    case Z:
        /*generate

            ZZ
             ZZ


        */

        if (
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[1][5] != ' ' ||
            map[1][6] != ' '
            ) {
                isPossible = false;
            }


        map[0][4] = 'Z';
        map[0][5] = 'Z';
        map[1][5] = 'X';
        map[1][6] = 'Z';

        currentPiece = 'Z';

        break;
    case S:
        /*generate

             SS
            SS


        */

        if (
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[1][4] != ' ' ||
            map[1][3] != ' '
            ) {
                isPossible = false;
            }


        map[0][4] = 'S';
        map[0][5] = 'S';
        map[1][4] = 'X';
        map[1][3] = 'S';

        currentPiece = 'S';

        break;
    case L:
        /*generate

            LLL
            L

        */


        if (
            map[0][3] != ' ' ||
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[1][3] != ' '
            ) {
                isPossible = false;
            }


        map[0][3] = 'X';
        map[0][4] = 'L';
        map[0][5] = 'L';
        map[1][3] = 'L';

        currentPiece = 'L';

        break;
    case J:
        /*generate

            J
            JJJ


        */

        if (
            map[0][3] != ' ' ||
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[1][5] != ' '
            ) {
                isPossible = false;
            }


        map[0][3] = 'J';
        map[0][4] = 'J';
        map[0][5] = 'X';
        map[1][5] = 'J';

        currentPiece = 'J';

        break;
    case T:
        /*generate

            TTT
             T

        */

        if (
            map[0][4] != ' ' ||
            map[0][5] != ' ' ||
            map[0][6] != ' ' ||
            map[1][5] != ' '
            ) {
                isPossible = false;
            }


        map[0][4] = 'T';
        map[0][5] = 'X';
        map[0][6] = 'T';
        map[1][5] = 'T';

        currentPiece = 'T';

        break;
        default:
            /*there was an error in the generation of the piece*/
            isPossible = false;
            break;
    }

    /*if "isPossible" is false, is not possible to continue the game
    because there is a collision between the current piece to generate
    and some other elements in the field

    true -> continue playing
    false -> game over
    */
    return isPossible;
}


bool Game::detectedLanding () {
    int scoreAddend = 0;

    /*landing the flying piece */
    for (int i = 0; i < v; i ++) {
        for (int j = 0; j < h; j ++) {
            if (isupper(map[i][j])) {
                map[i][j] = tolower(map[i][j]);
                if (map[i][j] == 'x')
                    map[i][j] = tolower(getCurrentPiece());

            }
        }
    }

    /*calculate the score*/
    scoreAddend += SCORE_PER_LANDING;
    scoreAddend += removeLines();

    /*increase the score*/
    increaseScore(scoreAddend);

    /*generate a new piece*/
    /*return if it is possible to continue the game*/
    return generatePiece();
}



/*methods added to run the genetic algorithm*/
int Game::getLinesRemoved() {
    return linesRemoved;
}

