#include<iostream>
#include<SDL2\SDL.h>

using namespace std;

int SW = 640, SH = 480;

int map[100]=
{
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
};
int mapx = 10, mapy = 10;


int main(int argc, char *argv[])
{
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) cout << "Video Error\n";
	else cout << "Video Ok\n";

	win = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SW,SH,SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

	int run = 1,fps = 60,r=0,s=1;

	int size = 40;

	SDL_Event e;

	while(run)
	{
		int start = SDL_GetTicks();
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
				run = 0;
			if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				run = 0;
		}
		SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren,255,0,255,255);

		for(int i=0;i<mapy;i++)
			for(int j=0;j<mapx;j++)
			{
				if(map[i*mapx + j])
				{
					SDL_Rect m = {i*40,j*40,size-1,size-1};
					SDL_RenderFillRect(ren,&m);
				}
			}

		int frame = SDL_GetTicks() - start;

		if(frame < 1000/fps)
			SDL_Delay(1000/fps - frame);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	return 0;
}