#include "Player.h"

Player::Player(SDL_Renderer* renderTarget, std::string filepath, float x, float y, int framesX, int framesY)
{
	SDL_Surface* surface = IMG_Load(filepath.c_str());

	if (surface == NULL)
	{
		std::cout << "ERROR" << std::endl;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
		{
			std::cout << "ERROR" << std::endl;
		}
	}
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &cropRect.w, &cropRect.h);


	positionRect.x = x;
	positionRect.y = y;
	
	textureWidth = cropRect.w;
	
	cropRect.w = 25;
	cropRect.h = 35;

	frameWidth = positionRect.w = 25;
	frameHeight = positionRect.h = 35;

	originX = frameWidth / 2; 
	originY = frameHeight / 2;

	radius = 13;

	isActive = false;

	static int playerNumber = 0;
	playerNumber++;

	if (playerNumber == 1)
	{
		keys[0] = SDL_SCANCODE_W;
		keys[1] = SDL_SCANCODE_S;
		keys[2] = SDL_SCANCODE_A;
		keys[3] = SDL_SCANCODE_D;
		keys[4] = SDL_SCANCODE_1;
	}
	else
	{
		keys[0] = SDL_SCANCODE_UP;
		keys[1] = SDL_SCANCODE_DOWN;
		keys[2] = SDL_SCANCODE_LEFT;
		keys[3] = SDL_SCANCODE_RIGHT;
		keys[4]	= SDL_SCANCODE_SLASH;
	}

	moveSpeed = 2.0f;
}


Player::~Player()
{
	SDL_DestroyTexture(texture);
}

void Player::Update(float delta, const Uint8* keyState)
{
	isActive = true;
	if (keyState[keys[2]])
	{
		positionRect.x -= moveSpeed;
		cropRect.y = frameHeight;

		if (keyState[keys[0]])
		{
			cropRect.y = frameHeight;
		}
		else if (keyState[keys[1]])
		{
			cropRect.y = frameHeight;
		}
		isActive = true;
	}
	else if (keyState[keys[3]])
	{
		positionRect.x += moveSpeed;
		cropRect.y = 0;

		if (keyState[keys[0]])
		{
			cropRect.y = 0;
		}
		else if (keyState[keys[1]])
		{
			cropRect.y = 0;
		}
		isActive = true;
	}
	if (keyState[keys[0]])
	{
		positionRect.y -= moveSpeed;
		isAttack = false;
		isActive = true;
	}
	else if (keyState[keys[1]])
	{
		positionRect.y += moveSpeed;
		isAttack = false;
		isActive = true;
	}
	else if (keyState[keys[2]])
	{
		cropRect.y = frameHeight;
		Dir = -1;
		isAttack = false;
		isActive = true;

	}
	else if (keyState[keys[3]])
	{
		cropRect.y = 0;
		Dir = 1;
		isAttack = false;
		isActive = true;
	}
	else if (keyState[keys[4]])
	{
		hold++;
		isActive = false;
		positionRect.w = 47;
		cropRect.w = 47;
		if (coolDown > 200)
		{
			if (Dir == 1)
			{
				cropRect.y = frameHeight * 4;
			}
			if (Dir == -1)
			{
				cropRect.y = frameHeight * 5;
			}

			frameCounter += delta;

			if (hold <= 5)
			{
				cropRect.x = 0;
			}
			if (frameCounter >= 0.02f)
			{
				frameCounter = 0;
				cropRect.x += 47;
				if (cropRect.x >= 1175)
				{
					cropRect.x = 0;
					coolDown = 0;
				}
			}
			isAttack = true;
		}
		
	}
	else
	{
		isActive = false;
		hold = 0;
	}

	if ( isActive == true && isAttack == false)
	{	
		hold++;
		coolDown++;

		frameCounter += delta;
		positionRect.w = 25;
		cropRect.w = 25;

		if (frameCounter >= 0.02f)
		{
			frameCounter = 0;
			cropRect.x += 25;
			if (cropRect.x >= 625)
			{
				cropRect.x = 0;
			}
		}
	}
	if( isAttack == false && isActive == false)
	{
		positionRect.w = 25;
		cropRect.w = 25;
		frameWidth = 25;

		if (Dir == 1) 
		{
			cropRect.y = frameHeight * 2;
		}
		if (Dir == -1)
		{
			cropRect.y = frameHeight * 3;
		}

		frameCounter += delta;

		if (frameCounter >= 0.4f)
		{
			frameCounter = 0;
			cropRect.x += 25;
			if (cropRect.x >= textureWidth)
			{
				cropRect.x = 0;
			}
		}
	}
	
}

void Player::inMap(Player& p)
{
	if (p.positionRect.x <= 80)
	{
		p.positionRect.x += moveSpeed;
	}
	if ((p.positionRect.x + p.frameWidth) >= LEVEL_WIDTH - 80)
	{
		p.positionRect.x -= moveSpeed;
	}
	if (p.positionRect.y <= 80)
	{
		p.positionRect.y += moveSpeed;
	}
	if ((p.positionRect.y + p.frameHeight) >= LEVEL_HEIGHT-80)
	{
		p.positionRect.y -= moveSpeed;
	}
}

void Player::Draw(SDL_Renderer* renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect);
}

bool Player::IntersectWith(Player &p)
{
	if (sqrt(pow(GetOriginX() - p.GetOriginX(), 2) + pow(GetOriginY() - p.GetOriginY(), 2)) >= (radius + p.GetRadius()))
	{
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			return false;
	}
	if (!isAttack)
	{
		SDL_SetTextureColorMod(texture, 255, 0, 0);
		return true;
		p.Lose = true;
	}

}

int Player::GetOriginX()
{
	return positionRect.x + originX;
}

int Player::GetOriginY()
{
	return positionRect.y + originY;
}

int Player::GetRadius()
{
	return radius;
}
