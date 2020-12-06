#include "game.h"

int main(){
    int gameState = INIT;
    int score = 0;
    
    for(;;) switch(gameState){
        case INIT: gameState = gameInit(); break;
        case PLAY: gameState =gamePlay(&score); break;
        case ENDING: gameState = gameEnding(&score); break;
        case DESTROY: gameState = gameDestroy(); break;
    }

    return 0;
}