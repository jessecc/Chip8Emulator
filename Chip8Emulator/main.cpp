#include <iostream>
#include <SDL.h>
#include "chip8.h"

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

typedef unsigned __int8 u8;

chip8 myChip8;
int modifier = 5;
u8 screenData[SCREEN_WIDTH][SCREEN_HEIGHT];
SDL_Window *gWindow = NULL;
SDL_Renderer *gRender = NULL;
bool pause = false;
bool quit = false;
const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

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

void keyDown(SDL_Event &e)
{
	if (e.key.keysym.sym == SDLK_ESCAPE)
		quit = true;

	if (e.key.keysym.sym == SDLK_PAUSE)
		pause = !pause;

	if (e.key.keysym.sym == SDLK_1)	myChip8.key[0x1] = 1;
	else if (e.key.keysym.sym == SDLK_2) myChip8.key[0x2] = 1;
	else if (e.key.keysym.sym == SDLK_3) myChip8.key[0x3] = 1;
	else if (e.key.keysym.sym == SDLK_4) myChip8.key[0xC] = 1;

	else if (e.key.keysym.sym == SDLK_q) myChip8.key[0x4] = 1;
	else if (e.key.keysym.sym == SDLK_w) myChip8.key[0x5] = 1;
	else if (e.key.keysym.sym == SDLK_e) myChip8.key[0x6] = 1;
	else if (e.key.keysym.sym == SDLK_r) myChip8.key[0xD] = 1;

	else if (e.key.keysym.sym == SDLK_a) myChip8.key[0x7] = 1;
	else if (e.key.keysym.sym == SDLK_s) myChip8.key[0x8] = 1;
	else if (e.key.keysym.sym == SDLK_d) myChip8.key[0x9] = 1;
	else if (e.key.keysym.sym == SDLK_f) myChip8.key[0xE] = 1;

	else if (e.key.keysym.sym == SDLK_z) myChip8.key[0xA] = 1;
	else if (e.key.keysym.sym == SDLK_x) myChip8.key[0x0] = 1;
	else if (e.key.keysym.sym == SDLK_c) myChip8.key[0xB] = 1;
	else if (e.key.keysym.sym == SDLK_v) myChip8.key[0xF] = 1;
}

void keyUp(SDL_Event &e)
{
	if (e.key.keysym.sym == SDLK_1)	myChip8.key[0x1] = 0;
	else if (e.key.keysym.sym == SDLK_2) myChip8.key[0x2] = 0;
	else if (e.key.keysym.sym == SDLK_3) myChip8.key[0x3] = 0;
	else if (e.key.keysym.sym == SDLK_4) myChip8.key[0xC] = 0;

	else if (e.key.keysym.sym == SDLK_q) myChip8.key[0x4] = 0;
	else if (e.key.keysym.sym == SDLK_w) myChip8.key[0x5] = 0;
	else if (e.key.keysym.sym == SDLK_e) myChip8.key[0x6] = 0;
	else if (e.key.keysym.sym == SDLK_r) myChip8.key[0xD] = 0;

	else if (e.key.keysym.sym == SDLK_a) myChip8.key[0x7] = 0;
	else if (e.key.keysym.sym == SDLK_s) myChip8.key[0x8] = 0;
	else if (e.key.keysym.sym == SDLK_d) myChip8.key[0x9] = 0;
	else if (e.key.keysym.sym == SDLK_f) myChip8.key[0xE] = 0;

	else if (e.key.keysym.sym == SDLK_z) myChip8.key[0xA] = 0;
	else if (e.key.keysym.sym == SDLK_x) myChip8.key[0x0] = 0;
	else if (e.key.keysym.sym == SDLK_c) myChip8.key[0xB] = 0;
	else if (e.key.keysym.sym == SDLK_v) myChip8.key[0xF] = 0;
}

void display()
{
	myChip8.emulateCycle();
	if (myChip8.drawFlag)
	{
		Uint32 tick1 = SDL_GetTicks();
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
		Uint32 tick2 = SDL_GetTicks() - tick1;
		if (tick2 < SCREEN_TICKS_PER_FRAME) SDL_Delay(SCREEN_TICKS_PER_FRAME - tick2);
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

	SDL_Event e;
	while (!quit)
	{
		Uint32 tick1 = SDL_GetTicks();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_KEYDOWN)
			{
				keyDown(e);
			}
			else if (e.type == SDL_KEYUP)
			{
				keyUp(e);
			}
		}

		if (pause) continue;

		display();
	}

	close();
	return 0;
}
