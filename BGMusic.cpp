#include "BGMusic.h"
#include <iostream>
BGMusic::BGMusic() {
	//Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    if (!loadMedia())
    {
        printf( "Failed to load media!\n");
    }
    if( Mix_PlayingMusic() == 0 )
	{
		//Play the music
		Mix_PlayMusic( bgmusic, -1 );
	}
}
BGMusic::~BGMusic() {
	closeMedia();
}


bool BGMusic::loadMedia()
{


    //Load music
    bgmusic = Mix_LoadMUS( "beat.mp3" );
    if( bgmusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void BGMusic::closeMedia()
{
    Mix_FreeMusic( bgmusic );
    bgmusic = NULL;
    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}

void BGMusic::update(const UpdateInfo& info) const {
    // Determine key strokes for pause/play
}

