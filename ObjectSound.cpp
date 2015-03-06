#include "ObjectSound.h"
#include <iostream>
#include "PhysicsCollider.h"

ObjectSound::ObjectSound(std::weak_ptr<GameObject> go)
  : Component(go)
{
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

    //Load sound effects
    scratch = Mix_LoadWAV( "scratch.wav" );
    if( bounce == NULL )
    {
        printf( "Failed to load bounce sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sound effects
    score = Mix_LoadWAV( "score.wav" );
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
    Mix_FreeChunk( score );
    score = NULL;
    Mix_FreeChunk( scratch );
    scratch = NULL;

    Mix_Quit();
    SDL_Quit();
}

void ObjectSound::collision(int val)
{
    switch (val)
    {
        case 1: 
        Mix_PlayChannel( -1, bounce, 0 );
        break;
        case 2: 
        Mix_PlayChannel( -1, score, 0 );
        break;
        case 3: 
        Mix_PlayChannel( -1, scratch, 0 );
        break;
    }
}

void ObjectSound::update(const UpdateInfo& info) {
    //Mix_PlayChannel( -1, bounce, 0 );
}

void ObjectSound::didCollide(PhysicsCollider& collider) {
  auto thisObjPtr = getGameObject().lock();
  auto otherObjPtr = collider.getGameObject().lock();
  if (otherObjPtr->getTag() == 0x1 || otherObjPtr->getTag() == 0x2) {
    // Other object is a ball. Only one of us should play the sound.
    if (thisObjPtr->getRandId() > otherObjPtr->getRandId()) {
      collision(1);
    }
  } 
  else {
    // Other object not a ball, so we must play a sound.
    collision(1);
  }
}

