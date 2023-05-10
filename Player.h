#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include "TileMap.h"
#include "CommonFunc.h"

class Player
{
private:
	SDL_Rect cropRect;
	float moveSpeed;
	float frameCounter;
	int textureWidth;
	bool isActive;
	SDL_Scancode keys[5];
	int Dir = 1;
	int originX, originY;

public:
	Player(SDL_Renderer* renderTarget, std::string filepath, float x, float y, int framesX, int framesY );
	~Player();

	SDL_Texture* texture;

	void Update(float delta, const Uint8* keyState);

	void Draw(SDL_Renderer* renderTarget);
	bool IntersectWith(Player &playe);
	void inMap(Player& p);

	bool Lose = false;

	int radius;
	int frameWidth, frameHeight;
	bool isAttack;
	int hold = 0;
	int coolDown = 0;

	SDL_Rect positionRect;

	int GetOriginX();
	int GetOriginY();
	int GetRadius();
};

