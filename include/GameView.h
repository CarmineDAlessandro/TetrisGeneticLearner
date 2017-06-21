#ifndef VIEW_H
#define VIEW_H


#include <Game.h>

#include <cstddef>

class GameView
{
    public:
        GameView(Game* game);
        /*the function to display the game*/
        static void drawGame();

    protected:

    private:
        static Game* game;

        static void drawScore();
        static void drawMap();
        static void drawCube(char el, float x, float y, float dim);


};

#endif // VIEW_H
