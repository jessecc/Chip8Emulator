#include <iostream>
#include <SDL.h>
#include "chip8.h"

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

typedef unsigned __int8 u8;

chip8 myChip8;
int modifier = 10;
u8 screenData[SCREEN_WIDTH][SCREEN_HEIGHT];
SDL_Window *gWindow = NULL;
SDL_Renderer *gRender = NULL;

void LogSDLError(const char* msg)
{
	printf("%s: %s\n", msg, SDL_GetError());
}

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LogSDLError("SDL_Init");
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*modifier, SCREEN_HEIGHT*modifier, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			LogSDLError("SDL_CreateWindow");
			success = false;
		}
		else
		{
			gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_ACCELERATED);
			if (gRender == nullptr)
			{
				LogSDLError("SDL_CreateRenderer");
				success = false;
			}
		}
	}

	return success;
}

void close()
{
	SDL_DestroyRenderer(gRender);
	gRender = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();

	return;
}

void display()
{
	myChip8.emulateCycle();
	if (myChip8.drawFlag)
	{
		SDL_SetRenderDrawColor(gRender, 0, 0, 0, 0);
		SDL_RenderClear(gRender);
		SDL_SetRenderDrawColor(gRender, 0xFF, 0xFF, 0xFF, 0x0);
		SDL_Rect rect;
		for (int y = 0; y < SCREEN_HEIGHT; ++y)
		{
			for (int x = 0; x < SCREEN_WIDTH; ++x)
			{
				if (myChip8.gfx[y * SCREEN_WIDTH + x] != 0)
				{
					rect.x = x * modifier;
					rect.y = y * modifier;
					rect.w = modifier;
					rect.h = modifier;
					SDL_RenderFillRect(gRender, &rect);
				}
			}
		}

		SDL_RenderPresent(gRender);
		myChip8.drawFlag = false;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: Chip8Emulator.exe chip8application\n");
		return 1;
	}

	if (!myChip8.loadApplication(argv[1]))
		return 1;

	if (!init())
	{
		return 2;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit == true;
			}
		}
		display();
	}

	close();
	return 0;
}