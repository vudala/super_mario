#include "game.h"

int main(){
    int gameState = INIT;
    int score = 0;
    
    for(;;) switch(gameState){
        case INIT: gameState = gameInit(); break;
        case PLAY: gameState = gamePlay(&score); break;
        case ENDING: gameState = gameEnding(&score); break;
        case DESTROY:
            gameDestroy();
            return 0;
            break;
    }

    return 0;
}