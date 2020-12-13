#include "game.h"
#include "sound.h"
#include "animation.h"

int main(){
    gameInit();
    
    int score = 0;

    ALLEGRO_BITMAP** screens = loadScreens();
    ALLEGRO_SAMPLE** samples = loadSamples();
    ALLEGRO_AUDIO_STREAM** tracks = loadTracks();

    int gameState = drawScreen(screens, START_SCREEN, samples, tracks);
    for(;;) switch(gameState){
        case PLAY: gameState = gamePlay(&score, screens, samples, tracks); break;
        case DESTROY:
            gameDestroy(screens, samples, tracks);
            return 0;
            break;
    }

    return 0;
}