#include<iostream>
#include<SDL2\SDL.h>

using namespace std;

int SW = 640, SH = 640;

class Snake
{
private:
	SDL_Rect s[100],f;
	int l,size;
public:
	Snake();
	void update(int d);
	void render(SDL_Renderer *r);
};

Snake::Snake()
{
	for(int i=0; i<100;i++)
		s[i] = {0,0,0,0};

	size = 16;
	l=3;

	int x = ((rand() % SW)>>4)<<4;
	int y = ((rand() % SH)>>4)<<4;

	f = {x,y,size,size};

	for(int i=0;i<l;i++)
	{
		s[i] = {208,208+(i*size),size,size};
	}
}

void Snake::update(int d)
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
			if(s[0].x + size> SW) s[0].x = 0;
			break;

		case 3:
			x = s[0].x;
			y = s[0].y;
			s[0].y += size;
			if(s[0].y + size> SH) s[0].y = 0;
			break;

		case 4:
			x = s[0].x;
			y = s[0].y;
			s[0].x -= size;
			if(s[0].x < 0) s[0].x = SW - size;
			break;
	}

	int ffl = 0;

	if(s[0].x == f.x && s[0].y == f.y)
	{
		f.x = ((rand() % SW)>>4)<<4;
		f.y = ((rand() % SH)>>4)<<4;
		ffl=1;
	}

	for(int i=1;i<l;i++)
	{
		int xp,yp;
		xp = s[i].x;
		yp = s[i].y;
		s[i].x = x;
		s[i].y = y;
		x = xp;
		y = yp; 
	}

	if(ffl)
		s[l++] = {x,y,size,size};
}

void Snake::render(SDL_Renderer *r)
{
	SDL_RenderFillRects(r,s,l);
	SDL_SetRenderDrawColor(r,0x25,0x0B,0x8D,255);
	SDL_RenderFillRect(r,&f);
	SDL_SetRenderDrawColor(r,127,0,0,255);
	SDL_RenderFillRect(r,&s[0]);
}

int main(int argc, char *argv[])
{
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) cout << "Video Error\n";
	else cout << "Video Ok\n";

	win = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SW,SH,SDL_WINDOW_SHOWN);
	ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

	int run = 1,fps = 60,dir = 1,del = 0;

	bool p = false;

	Snake s1;
	SDL_Event e;

	SDL_Rect r = {0,0,SW,SH};

	while(run)
	{
		int start = SDL_GetTicks();
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
				run = 0;
			if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				run = 0;

			if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.scancode == SDL_SCANCODE_W)
					if(dir!=3) dir = 1;
				
				if(e.key.keysym.scancode == SDL_SCANCODE_D)
					if(dir!=4) dir = 2;
				
				if(e.key.keysym.scancode == SDL_SCANCODE_S)
					if(dir!=1) dir = 3;
				
				if(e.key.keysym.scancode == SDL_SCANCODE_A)	
					if(dir!=2) dir = 4;

				if(e.key.keysym.scancode == SDL_SCANCODE_SPACE)
					p = !p;
			}
			if(e.type == SDL_MOUSEWHEEL)
			{
				if(e.wheel.y < 0)
					del++;
				if(e.wheel.y > 0)
					if(del-1>0)
						del--;
			}
		}
		SDL_SetRenderDrawColor(ren,0x98,0x80,0x4F,255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren,0x1D,0xC9,0x5C,255);

		s1.render(ren);
		if(p)
		{
			SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(ren,0,0,0,100);
			SDL_RenderFillRect(ren,&r);
		}
		
		SDL_RenderPresent(ren);

		if(!p) s1.update(dir);

		int frame = SDL_GetTicks() - start;

		if(frame < 1000/fps)
			SDL_Delay((1000/fps - frame) + del);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	return 0;
}