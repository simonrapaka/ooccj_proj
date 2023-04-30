#include<iostream>
#include<SDL2\SDL.h>

using namespace std;

int SW = 640, SH = 640;

class Snake
{
private:
	SDL_Rect s[100];
	int l,size;
public:
	Snake();
	void update(SDL_Renderer *r, int d);
	void render(SDL_Renderer *r);
};

Snake::Snake()
{
	for(int i=0; i<100;i++)
		s[i] = {0,0,0,0};

	size = 15;
	l=10;

	for(int i=0;i<l;i++)
	{
		s[i] = {200,200+(i*size),size,size};
	}
}

void Snake::update(SDL_Renderer *r, int d)
{
	int x,y;
	switch (d)
	{
	case 1:
		x = s[0].x;
		y = s[0].y;
		s[0].y -= size;
		if(s[0].y < 0) s[0].y = SH - size;
		break;

	case 2:
		x = s[0].x;
		y = s[0].y;
		s[0].x += size;
		if(s[0].x > SW) s[0].x = 0;
		break;

	case 3:
		x = s[0].x;
		y = s[0].y;
		s[0].y += size;
		if(s[0].y > SH) s[0].y = 0;
		break;

	case 4:
		x = s[0].x;
		y = s[0].y;
		s[0].x -= size;
		if(s[0].x < 0) s[0].x = SW - size;
		break;
	
	default:
		break;
	}

	for(int i=1;s[i].h != 0;i++)
	{
		int xp,yp;
		xp = s[i].x;
		yp = s[i].y;
		s[i].x = x;
		s[i].y = y;
		x = xp;
		y = yp; 
	}
}

void Snake::render(SDL_Renderer *r)
{
	SDL_RenderFillRects(r,s,l);
}

int main(int argc, char *argv[])
{
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) cout << "Video Error\n";
	else cout << "Video Ok\n";

	win = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SW,SH,SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

	int run = 1,fps = 20,r=0,s=1;

	int size = 64;

	int dir = 1;

	Snake s1;

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
			if(e.key.keysym.scancode == SDL_SCANCODE_W)
			{
				if(dir!=3) dir = 1;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_D)
			{
				if(dir!=4) dir = 2;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_S)
			{
				if(dir!=1) dir = 3;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_A)
			{
				if(dir!=2) dir = 4;
			}
		}
		SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren,255,0,255,255);

		s1.update(ren,dir);
		s1.render(ren);

		int frame = SDL_GetTicks() - start;

		if(frame < 1000/fps)
			SDL_Delay(1000/fps - frame);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	return 0;
}