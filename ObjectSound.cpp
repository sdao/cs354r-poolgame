#include "ObjectSound.h"
#include <iostream>
ObjectSound::ObjectSound() {
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

}
ObjectSound::~ObjectSound() {
	closeMedia();
}


bool ObjectSound::loadMedia()
{

    //Load sound effects
    bounce = Mix_LoadWAV( "PoolBall.wav" );
    if( bounce == NULL )
    {
        printf( "Failed to load bounce sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void ObjectSound::closeMedia()
{
    //Free the sound effects
    Mix_FreeChunk( bounce );
    bounce = NULL;

    Mix_Quit();
    SDL_Quit();
}

void ObjectSound::collision()
{
    Mix_PlayChannel( -1, bounce, 0 );
}

void ObjectSound::update(float deltaTime) const {
   	Mix_PlayChannel( -1, bounce, 0 );
}

