#include <SDL/SDL.h>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>

#define NUM_SMALL 	8
#define NUM_MED		8
#define NUM_BIG 	8
#define NUM_STARS (NUM_SMALL + NUM_MED + NUM_BIG)

#define SMALL_RADIUS 1
#define MED_RADIUS 4
#define BIG_RADIUS 8

#define SMALL_SPEED -1
#define MED_SPEED -2
#define BIG_SPEED -3

struct tstar {
    int x;
    int y;
    unsigned int r;
    int x_speed;
    int y_speed;
};

int FPS = 60;    //Assign an FPS
int NextTick , interval ;

//Grabbed this function from somewhere, I don't remember where
SDL_Surface* CreateSurface(Uint32 flags, int width, int height, const SDL_Surface* display)
{
  // 'display' is the surface whose format you want to match
  //  if this is really the display format, then use the surface returned from SDL_SetVideoMode

  const SDL_PixelFormat& fmt = *(display->format);
  return SDL_CreateRGBSurface(flags,width,height,
                  fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
}

// Initialize FPS_Fn()
void FPS_Initial(void) {
	NextTick = 0 ;
	interval = 1 * 1000 / FPS ; 
	return;
}

// FPS limiter function. Called from main loop.
void FPS_Fn(void) {
    if ( NextTick > SDL_GetTicks( ) ) SDL_Delay( NextTick - SDL_GetTicks( ) );
    NextTick = SDL_GetTicks( ) + interval ;
    return;
}

//Mega Man 2 stage
void buildStage(SDL_Surface &surface, int w, int h)
{
	//top initialization is from top to bottom
	SDL_Rect cyanTop1, cyanTop2;

	//bottom initialization is from bottom to top
	SDL_Rect cyanBot1, cyanBot2;
	SDL_Rect cyanMid;

	SDL_Rect blueRect;

	blueRect.w = w;
	blueRect.h = h;
	blueRect.x = 0;
	blueRect.y = 0;


	cyanTop1.w = w;
	cyanTop1.h = blueRect.h / 50;
	cyanTop1.x = 0;
	cyanTop1.y = blueRect.y + cyanTop1.h;

	cyanTop2.w = w;
	cyanTop2.h = blueRect.h / 50;
	cyanTop2.x = 0;
	cyanTop2.y = blueRect.y + cyanTop2.h * 3;

	cyanBot1.w = w;
	cyanBot1.h = blueRect.h / 50;
	cyanBot1.x = 0;
	cyanBot1.y = (blueRect.y + blueRect.h) - (cyanBot1.h * 2);

	cyanBot2.w = w;
	cyanBot2.h = blueRect.h / 50;
	cyanBot2.x = 0;
	cyanBot2.y = (blueRect.y + blueRect.h) - (cyanBot1.h * 4);


	cyanMid.w = w;
	//height is the y-distance between the top of cyanbot2 (the top cyan bar on the bottom) 
	//and the bottom of cyantop2 (the bottom cyan bar on the top),
	//minus the height of 4 cyan bars on the top and the bottom.
	cyanMid.h = (cyanBot2.y - (cyanBot2.h * 4)) - (cyanTop2.y + cyanTop2.h * 5);
	cyanMid.x = 0;
	cyanMid.y = (cyanTop2.y + cyanTop2.h * 5);

	SDL_FillRect(&surface, &blueRect, SDL_MapRGB(surface.format, 0,0,255));
	SDL_FillRect(&surface, &cyanTop1, SDL_MapRGB(surface.format, 0,255,255));
	SDL_FillRect(&surface, &cyanTop2, SDL_MapRGB(surface.format, 0,255,255));
	SDL_FillRect(&surface, &cyanBot1, SDL_MapRGB(surface.format, 0,255,255));
	SDL_FillRect(&surface, &cyanBot2, SDL_MapRGB(surface.format, 0,255,255));
	SDL_FillRect(&surface, &cyanMid, SDL_MapRGB(surface.format, 0,255,255));
}

void setupStar(struct tstar &star, unsigned int r, 
		unsigned int x_pos, 
		unsigned int y_pos,
		int x_speed)
{


	//generate star for region and size
	star.x = x_pos;
	star.y = y_pos;
	star.r = r; //randomize size
	star.x_speed = x_speed; //dependent on size, bigger stars move slower
	star.y_speed = 0;
}

void animateStar(SDL_Surface *screen, struct tstar &star, 
	unsigned int min_x, 
	unsigned int min_y, 
	unsigned int max_x, 
	unsigned int max_y) {
	
	//update x/y
	star.x += star.x_speed;
	star.y += star.y_speed;
	if (star.x <= min_x + star.r) star.x = max_x;
	//if (star.x >= max_x - star.r) star.x = min_x + star.r;
	//if (star.y <= min_y + star.r) star.y = max_y - star.r;
	//if (star.y >= max_y - star.r) star.y = min_y + star.r;
	
	filledCircleRGBA(screen, star.x, star.y, star.r, 255, 255, 255, 255); 
}

void animateText(SDL_Surface *screen, char* text, unsigned int pos) {

}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	SDL_Event event;
	bool bRun = 1;
	int pidx = 0 , aTick = 0;
	tstar stars[NUM_STARS];

	//Probably should make this a commandline option but whatever I dont care this is a toy
	std::string text = "PAT MAN";
	unsigned int text_pos = 0; //This should be an iterator which we define and use later on, or maybe not even exist at all who knows


	SDL_Surface *screen;
	SDL_Rect stageRect;
	SDL_Rect nameRect;
	TTF_Font *font;
	Mix_Music *music;


	atexit(SDL_Quit);

	SDL_WM_SetCaption("MM2 Bossintro", NULL);

//#ifdef WINDOWED
//screen = SDL_SetVideoMode( 640 , 480 , 32 , SDL_DOUBLEBUF);
//#else
	screen = SDL_SetVideoMode( 1920 , 1080 , 32 , SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT|SDL_FULLSCREEN);
//#endif
	
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	printf("info: %d %d\n", info->current_w, info->current_h);

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		printf("failed to open sdl_mixer\n");
		return 1;
	}

	if (TTF_Init() < 0) {
		printf("failed to init ttf\n");
		return 1;
	    // Handle error...
	}

	stageRect.w = info->current_w;
	stageRect.h = info->current_h / 3.5;
	stageRect.x = 0;
	stageRect.y = (info->current_h / 2) - (stageRect.h / 2);

	SDL_Surface *stage = CreateSurface(0, stageRect.w, stageRect.h, screen);
	buildStage(*stage, stageRect.w, stageRect.h);

	

	/*
		Set up stars
	*/
	//small
	int i = 0;
	for (int k = i; i < k+NUM_SMALL; i++) {
		if ((rand()%101+1) > 50) {
			//top
			setupStar(stars[i], SMALL_RADIUS, rand()%info->current_w, rand()%(stageRect.y - SMALL_RADIUS), SMALL_SPEED);
		} else {
			//bottom
			int y = rand()%(info->current_h - (stageRect.y + stageRect.h + SMALL_RADIUS)) + (stageRect.y + stageRect.h + SMALL_RADIUS);
			setupStar(stars[i], SMALL_RADIUS, rand()%info->current_w, y, SMALL_SPEED);
		}
		//printf("%i, %i, %i, %i\n", stars[i].x, stars[i].y, stars[i].r, stars[i].x_speed);
	}

	//medium
	for (int k = i; i < k + NUM_MED; i++) {
		if ((rand()%101+1) > 50) {
			//top
			setupStar(stars[i], MED_RADIUS, rand()%info->current_w, rand()%(stageRect.y - MED_RADIUS), MED_SPEED);
		} else {
			//bottom
			int y = rand()%(info->current_h - (stageRect.y + stageRect.h + MED_RADIUS)) + (stageRect.y + stageRect.h + MED_RADIUS);
			setupStar(stars[i], MED_RADIUS, rand()%info->current_w, y, MED_SPEED);
		}
		//printf("%i, %i, %i, %i\n", stars[i].x, stars[i].y, stars[i].r, stars[i].x_speed);
	}

	//big
	for (int k = i; i < k + NUM_BIG; i++) {
		if ((rand()%101+1) > 50) {
			//top
			setupStar(stars[i], BIG_RADIUS, rand()%info->current_w, rand()%(stageRect.y - BIG_RADIUS), BIG_SPEED);
		} else {
			//bottom
			int y = rand()%(info->current_h - (stageRect.y + stageRect.h + BIG_RADIUS)) + (stageRect.y + stageRect.h + BIG_RADIUS);
			setupStar(stars[i], BIG_RADIUS, rand()%info->current_w, y, BIG_SPEED);
		}
		//printf("%i, %i, %i, %i\n", stars[i].x, stars[i].y, stars[i].r, stars[i].x_speed);
	}


	music = Mix_LoadMUS("./mm2.ogg");
	if (music == NULL) return false;

	font = TTF_OpenFont("ArcadeClassic.ttf", info->current_h / 32);
	
	int text_w, text_h;
	TTF_SizeText(font, text.c_str(), &text_w, &text_h);

	nameRect.w = text_w;
	nameRect.h = text_h;
	nameRect.x = info->current_w / 2 - nameRect.w / 2;
	nameRect.y = stageRect.y + stageRect.h - ((stageRect.h / 50) * 16);

	FPS_Initial( );
	Mix_PlayMusic(music, 1);

	while(bRun && Mix_PlayingMusic() != 0) {
		SDL_FillRect(screen , NULL ,SDL_MapRGB(screen->format,0,0,0));
		SDL_BlitSurface(stage, NULL, screen, &stageRect);

		for (int i=0; i < NUM_STARS; i++) {
			//printf("%i, %i, %i, %i\n", stars[i].x, stars[i].y, stars[i].r, stars[i].x_speed);
			animateStar(screen, stars[i], 0, 0, info->current_w, info->current_h);
		}


		if(aTick%10 == 0) {
			pidx++;

			//make sure we start drawing the text at the right time
			if (pidx > 180) {
				SDL_Surface *name;
				SDL_Color c = {255,255,255,255};
				name = TTF_RenderText_Solid(font, text.substr(0,text_pos).c_str(), c);
				SDL_BlitSurface(name, NULL, screen, &nameRect);

				//TODO: handle timing based on length of text
				if (pidx%20 == 0) {
					if (text_pos < sizeof(text))
						text_pos++;
				}
			}
		}




		SDL_Flip(screen);
		FPS_Fn(); //limit framerate

		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					bRun = 0;
					break;
				case SDL_QUIT:
					bRun = 0;
					break;
				default:
					break;
			}
		};
	    
    };

    return 0;
}
