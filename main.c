#include "game.h"
#include "sound.h"
#include "animation.h"

int main(){
    gameInit();
    
    int score = 0;

    ALLEGRO_BITMAP** screens = loadScreens();
    ALLEGRO_SAMPLE** samples = loadSamples();

    int gameState = drawScreen(screens, START_SCREEN, samples, NULL);
    for(;;) switch(gameState){
        case PLAY: gameState = gamePlay(&score, screens, samples); break;
        case DESTROY:
            gameDestroy();
            return 0;
            break;
    }

    return 0;
}