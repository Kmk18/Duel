#include "CommonFunc.h"
#include "Player.h"

SDL_Window* window = NULL;
SDL_Renderer* renderTarget = NULL;
SDL_Color color = { 255,255,255 };

Texture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

bool Texture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderTarget, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderTarget, mTexture, clip, &renderQuad, angle, center, flip);
}

void Tile::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mBox))
	{
		//Show the tile
		gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
	}
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Load tile texture
	if (!gTileTexture.loadFromFile("assets/tile.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gTileTexture.free();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderTarget);

	window = nullptr;
	renderTarget = nullptr;

	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("assets/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_RED].x = 0;
			gTileClips[TILE_RED].y = 0;
			gTileClips[TILE_RED].w = TILE_WIDTH;
			gTileClips[TILE_RED].h = TILE_HEIGHT;

			gTileClips[TILE_GREEN].x = 0;
			gTileClips[TILE_GREEN].y = 80;
			gTileClips[TILE_GREEN].w = TILE_WIDTH;
			gTileClips[TILE_GREEN].h = TILE_HEIGHT;

			gTileClips[TILE_BLUE].x = 0;
			gTileClips[TILE_BLUE].y = 160;
			gTileClips[TILE_BLUE].w = TILE_WIDTH;
			gTileClips[TILE_BLUE].h = TILE_HEIGHT;

			gTileClips[TILE_TOPLEFT].x = 80;
			gTileClips[TILE_TOPLEFT].y = 0;
			gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
			gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_LEFT].x = 80;
			gTileClips[TILE_LEFT].y = 80;
			gTileClips[TILE_LEFT].w = TILE_WIDTH;
			gTileClips[TILE_LEFT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMLEFT].x = 80;
			gTileClips[TILE_BOTTOMLEFT].y = 160;
			gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

			gTileClips[TILE_TOP].x = 160;
			gTileClips[TILE_TOP].y = 0;
			gTileClips[TILE_TOP].w = TILE_WIDTH;
			gTileClips[TILE_TOP].h = TILE_HEIGHT;

			gTileClips[TILE_CENTER].x = 160;
			gTileClips[TILE_CENTER].y = 80;
			gTileClips[TILE_CENTER].w = TILE_WIDTH;
			gTileClips[TILE_CENTER].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOM].x = 160;
			gTileClips[TILE_BOTTOM].y = 160;
			gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

			gTileClips[TILE_TOPRIGHT].x = 240;
			gTileClips[TILE_TOPRIGHT].y = 0;
			gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_RIGHT].x = 240;
			gTileClips[TILE_RIGHT].y = 80;
			gTileClips[TILE_RIGHT].w = TILE_WIDTH;
			gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMRIGHT].x = 240;
			gTileClips[TILE_BOTTOMRIGHT].y = 160;
			gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

int CheckPaddleCollision(Ball const& ball, Player &p)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	float paddleLeft = p.positionRect.x;
	float paddleRight = p.positionRect.x + p.radius + p.GetRadius();
	float paddleTop = p.positionRect.y;
	float paddleBottom = p.positionRect.y + p.radius + p.GetRadius();

	if (ballLeft >= paddleRight)
	{
		return 0;
	}

	if (ballRight <= paddleLeft)
	{
		return 0;
	}

	if (ballTop >= paddleBottom)
	{
		return 0;
	}

	if (ballBottom <= paddleTop)
	{
		return 0;
	}
	if (p.isAttack)
	{
		return 1;
	}
	else if (!p.isAttack)
	{
		return 2;
	}


}

int main(int argc, char* args[])
{
	int currentTime = 0;
	int prevTime = 0;
	float delta = 0.0f;
	const Uint8* keyState;

	bool success = true;

	TTF_Init();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow("Duel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderTarget == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderTarget, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	Ball ball(
		Vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),
		Vec2(BALL_SPEED, 0.0f));

	Tile* tileSet[TOTAL_TILES];

	//Load media
	if (!loadMedia(tileSet))
	{
		printf("Failed to load media!\n");
	}

	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Player player1(renderTarget, "assets/allmotion1.png",200, SCREEN_HEIGHT/2, 25, 6);
	Player player2(renderTarget, "assets/allmotion2.png", 1000, SCREEN_HEIGHT/2, 25, 6);

	bool isRunning = true;
	SDL_Event ev;

	while (isRunning)
	{
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		delta = (currentTime - prevTime) / 1000.0f;

		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;
		}

		keyState = SDL_GetKeyboardState(NULL);

		player1.Update(delta, keyState);
		player2.Update(delta, keyState);

		player1.inMap(player1);
		player2.inMap(player2);

		if (player1.IntersectWith(player2) || player2.IntersectWith(player1))
		{
			if (MessageBox(NULL, L"DRAW", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				player1.positionRect.x = 200;
				player1.positionRect.y = 360;
				player2.positionRect.x = 1000;
				player2.positionRect.y = 360;
				ball.position.x = SCREEN_WIDTH / 2;
				ball.position.y = SCREEN_HEIGHT / 2;
				ball.timer = 0;
			}
		}

		SDL_RenderClear(renderTarget);

		//Render level
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			tileSet[i]->render(camera);
		}

		// Update the ball position
		ball.Update();

		if (CheckPaddleCollision(ball, player2) == 1 ||
			CheckPaddleCollision(ball, player1) == 1)
		{
			ball.velocity.x = -ball.velocity.x;
		}
		else if (CheckPaddleCollision(ball, player2) == 2)
		{
			player2.Lose = true;
		}
		else if (CheckPaddleCollision(ball, player1) == 2)
		{
			player1.Lose = true;
		}

		ball.touchWall(ball);

		// Draw the ball
		ball.Draw(renderTarget);

		player1.Draw(renderTarget);
		player2.Draw(renderTarget);

		if (player1.Lose == true)
		{
			if (MessageBox(NULL, L"Player 2 wins", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				player1.positionRect.x = 200;
				player1.positionRect.y = 360;
				player2.positionRect.x = 1000;
				player2.positionRect.y = 360;
				ball.position.x = SCREEN_WIDTH / 2;
				ball.position.y = SCREEN_HEIGHT / 2;
				player1.Lose = false;
				ball.timer = 0;
			}
		}
		if (player2.Lose == true)
		{
			if (MessageBox(NULL, L"Player 1 wins", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				player1.positionRect.x = 200;
				player1.positionRect.y = 360;
				player2.positionRect.x = 1000;
				player2.positionRect.y = 360;
				ball.position.x = SCREEN_WIDTH / 2;
				ball.position.y = SCREEN_HEIGHT / 2;
				player2.Lose = false;
				ball.timer = 0;
			}
		}

		SDL_RenderPresent(renderTarget);
	}

	close(tileSet);

	return 0;
}