#ifndef MODEL_H
#define MODEL_H

#define MAX_LEVEL 15
#define SCORE_PER_LANDING 2
#define SCORE_PER_LINE 10

enum Piece{LINE = 0, SQUARE = 1, S = 2, Z = 3, L = 4, J = 5, T = 6};



/*the class of the game*/
class Game
{
    public:
        /*constructor*/
        Game();

        /*horizontal dimension of the map*/
        static const int h = 10;
        /*vertical dimension of the map*/
        static const int v = 18;

        /*getters*/
        int getScore ();
        int getLevel ();
        const float getSpeed ();
        char** getMap ();
        char getCurrentPiece ();

        /*flag getters*/
        bool isInPause ();
        bool isPlaying ();

        /*control functions*/
        void moveLeft ();
        void moveRight ();
        void allBottom ();
        void stepBottom ();
        void rotatePiece ();

        /*pause - restart*/
        void pause ();
        void unpause ();

        /*added methods needed for the run of the genetic algorithm*/
        int getLinesRemoved();

    protected:

    private:
        /*the score of the game */
        int score;
        /*there are 15 levels*/
        int level;
        /*the array speed-score per level*/
        static const float speed [15][2];
        /*the map of the game*/
        char **map;
        /*the current flying piece*/
        char currentPiece;

        bool isPaused;
        bool playing;
        bool isLanded;



        /*the speed is automatic calculated from the score*/
        void increaseScore (int addend);
        /*this function removes the full lines from the map and returns the score obtained in this way*/
        int removeLines ();
        /*this function generate a new piece returning false if it is not possible to generate it*/
        bool generatePiece ();
        /*this function detect the landing of a "flying" block*/
        bool detectedLanding ();



        /*to the genetic algorithm*/
        /*it's better to save a copy*/
        int linesRemoved;

};

#endif // MODEL_H
