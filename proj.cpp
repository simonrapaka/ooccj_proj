#include<iostream>
#include<SDL2\SDL.h>
#include<SDL2\SDL_ttf.h> //for text rendering

using namespace std;

int fail=0;

//For Windowed Operation
int SW = 640, SH = 640, w = 4,m = 0;

//For FullScreen Operation
// int SW = 1280,SH = 720,w = 1,m = 1;

class Snake
{
private:
	SDL_Rect s[100],f;
	int l,size;
public:
	Snake();
	void update(int d);
	void render(SDL_Renderer *r);
	void render(SDL_Renderer *r, TTF_Font *font, SDL_Color *col, SDL_Surface *surf, SDL_Texture *text, SDL_Rect *score, SDL_Rect *bar);
	void renderf(SDL_Renderer *r);
	int score();
};

Snake::Snake()
{
	for(int i=0; i<100;i++) //Initializes all values to 0
		s[i] = {0,0,0,0};

	size = 16; //sets the size of each cell
	l=3; //sets the length

	int x = ((rand() % SW)>>4)<<4; //generates a random location for the food
	int y = ((rand() % SH)>>4)<<4;

	f = {x,y,size,size}; //initializes food rectangle with the generated location

	for(int i=0;i<l;i++) //initializes the first l rectangles of the snake array to a location and size
	{
		s[i] = {208,208+(i*size),size,size};
	}
}

//returns the score
int Snake::score()
{
	return l-3;
}


//updates the location of the snake each frame
void Snake::update(int d)
{
	int x,y;
	switch (d) // moves the head of the snake in a certain direction 1: up 2: right 3: down 4: left
	{
		case 1:
			x = s[0].x; // stores the location of head before updating it
			y = s[0].y;
			s[0].y -= size;
			if(s[0].y < 2*size) s[0].y = SH - size; //if head's y corrdinate is above the top of the window, changes its location to bottom
			break;

		case 2:
			x = s[0].x;
			y = s[0].y;
			s[0].x += size;
			if(s[0].x + size> SW) s[0].x = 0; //if head's x coordinate is grater than the width of the window, changes it to zero
			break;

		case 3:
			x = s[0].x;
			y = s[0].y;
			s[0].y += size;
			if(s[0].y + size> SH) s[0].y = 2*size;
			break;

		case 4:
			x = s[0].x;
			y = s[0].y;
			s[0].x -= size;
			if(s[0].x < 0) s[0].x = SW - size;
			break;
	}

	int ffl = 0; //flag to check whether food is eaten or not

	if(s[0].x == f.x && s[0].y == f.y) // checks whether the snake head is on top of a food rectangle
	{
		f.x = ((rand() % SW)>>4)<<4;
		f.y = ((rand() % SH)>>4)<<4;
		

		while(true)
		{
			int c=0; // flag to check 
			for(int i=0;i<l;i++) //checks whether the new food spawn location is the same as a snake cell
			{
				if(f.x == s[i].x && f.y == s[i].y) //if true generates a new spawn location for the food and sets c to 1
				{
					f.x = ((rand() % SW)>>4)<<4;
					f.y = ((rand() % SH)>>4)<<4;
					c=1;
				}
			}
			if(c) continue; // runs through the loop again if c is 1
			else break;
		}

		ffl=1; //Set food flag to 1
	}

	if(f.y < 2*size) f.y = 2*size;

	//goes through all the active rectangles of the snake and moves them to their predecessors previous location
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

	//checks whether the head of the snake is on a snake cell
	for(int i=1;i<l;i++)
	{
		if(s[0].x == s[i].x && s[0].y == s[i].y)
			fail=1;// Set fail flag to 1
	}

	//if a food rectangle is eaten, increases the snake length by 1
	if(ffl)
		s[l++] = {x,y,size,size};
}

void Snake::render(SDL_Renderer *r)
{
	SDL_RenderFillRects(r,s,l); //render snake
	SDL_SetRenderDrawColor(r,0x25,0x0B,0x8D,255);
	SDL_RenderFillRect(r,&f); //render food
	SDL_SetRenderDrawColor(r,127,0,0,255);
	SDL_RenderFillRect(r,&s[0]); //render head of snake
}

void Snake::render(SDL_Renderer *r, TTF_Font *font, SDL_Color *col, SDL_Surface *surf, SDL_Texture *text, SDL_Rect *score, SDL_Rect *bar)
{
	char sc[15];

	sprintf(sc,"Score: %d", l-3);

	surf = TTF_RenderText_Solid(font,sc,*col);
	text = SDL_CreateTextureFromSurface(r,surf);

	SDL_SetRenderDrawColor(r,0,127,0,255);
	SDL_RenderFillRect(r,bar);

	SDL_RenderCopy(r,text,NULL,score);
}

void Snake::renderf(SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r,127,0,0,255); //set color to red
	SDL_RenderFillRects(r,s,l); //render snake
}

int main(int argc, char *argv[])
{
	SDL_Window *win = NULL;
	SDL_Renderer *ren = NULL;
	SDL_Surface *sur = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) cout << "Video Error\n";
	else cout << "Video Ok\n";

	win = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SW,SH,w); //Create Window with certain parameters
	ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED); //Create Renderer that points to the previously created window

	if(m) //if m is 1 make mouse cursor invisible
	{
		if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0) cout << "Mouse Capture Error\n";
		else cout << "Mouse Capture OK\n";
	}

	int run = 1,fps = 60,dir = 1,del=0; // initialize with direction up and 0ms delay

	bool p = false; //pause flag set to false

	Snake s1; //Create Snake object
	SDL_Event e; //Create an event to check inputs

	int scw = 150, sch = 30;
	SDL_Rect r = {0,0,SW,SH}; //Screen size rectangle for dimming
	SDL_Rect score = {SW/2-scw/2,4,scw,sch}; //Score rectangle
	SDL_Rect bar = {0,0,SW,32}; //Top bar

	TTF_Font *sco =  TTF_OpenFont("Minecrafter.Reg.ttf",14);
	if(sco==NULL) {cout << "Font not found\n"; run = 0;} 
	SDL_Color scr = {80,80,80};
	SDL_Surface *scre;
	SDL_Texture *scoret;

	while(run)
	{
		int start = SDL_GetTicks();
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT) //if the close button or Alt+F4 is pressed
				run = 0;

			if(e.type == SDL_KEYDOWN) //if a key is pressed
			{
				if(e.key.keysym.scancode == SDL_SCANCODE_W)
					{if(dir!=3 && !p) dir = 1; break;}
				
				if(e.key.keysym.scancode == SDL_SCANCODE_D)
					{if(dir!=4 && !p) dir = 2; break;}
				
				if(e.key.keysym.scancode == SDL_SCANCODE_S)
					{if(dir!=1 && !p) dir = 3; break;}
				
				if(e.key.keysym.scancode == SDL_SCANCODE_A)	
					{if(dir!=2 && !p) dir = 4; break;}

				if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) //if escape is pressed while running, set pause flag to true, if pressed while paused, set to false
					p = !p;

				if(e.key.keysym.scancode == SDL_SCANCODE_X) //if x is pressed while paused set run flag to zero exiting the game
					if(p) run = 0;
			}
			
			if(e.type == SDL_MOUSEWHEEL)
			{
				if(e.wheel.y < 0) //if scroll wheel is scrolled down increase the delay after rendering each frame slowin down the game
					del+=5;
				if(e.wheel.y > 0) //if scroll wheel is scrolled up decrease the delay, speeding up the game
					if(del-5 >= 0)
						del-=5;
			}
		}
		SDL_SetRenderDrawColor(ren,0x98,0x80,0x4F,255); //Render Background
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren,0x1D,0xC9,0x5C,255); //Set color to a light green

		s1.render(ren); //Render snake, snake head and food

		s1.render(ren,sco,&scr,scre,scoret,&score,&bar);

		if(p) //if paused dim the screen
		{
			SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(ren,0,0,0,100);
			SDL_RenderFillRect(ren,&r);
		}

		if(fail)
		{
			//Render Snake
			s1.renderf(ren);

			//Render a red overlay on the screen
			SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND); 
			SDL_SetRenderDrawColor(ren,120,0,0,100);
			SDL_RenderFillRect(ren,&r);
		}

		SDL_RenderPresent(ren); //Actual point in the program where the screen is updated

		if(!p && !fail) s1.update(dir); //update the snake and food location if game isn't paused and isn't in a fail state

		if(fail)
		{
			//Wait for 1 second(1000ms) and exit the game
			SDL_Delay(1000);
			break;
		}

		int frame = SDL_GetTicks() - start;

		if(frame < 1000/fps) //if the amount of time taken to render is less than one frame, delay until one frame's worth of time has passed
			SDL_Delay((1000/fps - frame) + del);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	cout << "Score: " << s1.score() << "\n";

	return 0;
}