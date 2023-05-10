#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;

const float BALL_SPEED = 5.0f;

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

class Vec2
{
public:
	Vec2()
		: x(0.0f), y(0.0f)
	{}

	Vec2(float x, float y)
		: x(x), y(y)
	{}

	Vec2 operator+(Vec2 const& rhs)
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2& operator+=(Vec2 const& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Vec2 operator*(float rhs)
	{
		return Vec2(x * rhs, y * rhs);
	}

	float x, y;
};

class Ball
{
public:
	Ball(Vec2 position, Vec2 velocity)
		: position(position), velocity(velocity)
	{
		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = BALL_WIDTH;
		rect.h = BALL_HEIGHT;
	}

	int timer = 0;

	void Update()
	{
		timer++;
		if (timer > 500)
		{
			position += velocity;
		}
	}

	void Draw(SDL_Renderer* renderer)
	{
		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);

		SDL_RenderFillRect(renderer, &rect);
	}

	void touchWall(Ball &b)
	{
		if (b.position.x < 80)
		{
			b.velocity.x = -b.velocity.x;
			b.position.y += (rand() % 400 - 200);
		}
		if (b.position.x + BALL_WIDTH > SCREEN_WIDTH-80)
		{
			b.velocity.x = -b.velocity.x;
			b.position.y += (rand() % 400 - 200);
		}
		if (b.position.y < 80)
		{
			b.position.y += (200 + rand() % 100 );
		}
		if (b.position.y + BALL_HEIGHT > SCREEN_HEIGHT-80)
		{
			b.position.y += - (200 + rand() % 100);
		}
	}

	Vec2 position;
	Vec2 velocity;
	SDL_Rect rect{};
};


#endif