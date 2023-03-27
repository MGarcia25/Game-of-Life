/**
 * @file gl.c
 *
 * @author Mariano Garcia
 *
 * @date 4/24/2020
 *
 * Assignment: Lab 10
 *
 * @brief The main file for game of life
 *
 * @details All of the code that will be used to Game of Life
 *
 * @bugs none
 *
 * @todo none
 */
#include <stdlib.h>
#include <unistd.h> /* used for getopt */
#include "SDL2/SDL.h"
#include "sdl.h"
#include "life.h"

int main(int argc, char *argv[])
{
	int width = 800;
	int height = 600;
	int sprite_size = 4; /* either 2, 4, 8, or 16 */
	int m = (width/sprite_size/2);
	int n = (height/sprite_size/2);
        /* colors are RGB model valid values [0, 255] */
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 250;
    const char *filename = NULL;
    char edge = 'K'; // default torus
    struct sdl_info_t sdl_info; /* this is needed to graphically display the game */

    /* parse command line argument */
    char opt;
    while ((opt = getopt(argc, argv, ":w:h:r:g:b:s:f:o:e:H")) != -1)
    {
        switch (opt) {
        case 'w':
            sscanf(optarg,"%d", &width);
            break;
        case 'h':
            sscanf(optarg,"%d", &height);
            break;
        case 'r':
            sscanf(optarg,"%c", &red);
            break;
        case 'g':
            sscanf(optarg,"%c", &green);
            break;
        case 'b':
            sscanf(optarg,"%c", &blue);
            break;
        case 's':
            sscanf(optarg,"%d", &sprite_size);
            break;
        case 'f':
            filename = optarg;
            break;
        case 'o':
            sscanf(optarg,"%d,%d", &m, &n);
            break;
        case 'e':
            if (0 ==strcmp(optarg,"hedge")) edge = 'H';
            if (0 ==strcmp(optarg,"torus")) edge = 'T';
            if (0 ==strcmp(optarg,"klein")) edge = 'K';
            break;
        case 'H':
            printf("usage: life -w 640 -h 480 -r 100 -g 150 -b 200 -s 2 -f "
                    "glider_106.lif -o 150,200 -e torus\n");
            exit(1);
            break;
        case '?':
        default:
            break;
        }
    }

    printf("Settings: Height:%d, Width:%d, RGB:%d,%d,%d, "
            "Sprite:%d, File:%s, Initial:%d,%d, Edge:%c\n",
        height, width, red, green, blue, sprite_size, filename, m,n, edge);
    /* set up SDL -- works with SDL2 */
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

	/* your life initialization code here */
    // calculate size of life matrix
    int lifeRows = width/sprite_size;
    int lifeCols = height/sprite_size;
	unsigned char **lifeA = alloc_life(lifeRows, lifeCols);
    unsigned char **lifeB = alloc_life(lifeRows, lifeCols);

    reset_life(lifeA, lifeRows, lifeCols);
    reset_life(lifeB, lifeRows, lifeCols);

    if (filename == NULL) {
        filename = "lifs/glider_106.lif";
    }
    // all the positions would be relative to this
    Cell curPos;
    curPos.row = n;
    curPos.col = m;
    if (! init_life_from_file(lifeA, lifeRows, lifeCols, edge, curPos, filename))
    {
        exit(2);
    }

    // Hold on for 1 sec to show initial drawing
    sdl_render_life(&sdl_info, lifeA);
    sleep(1);

    /* Main loop: loop forever. */
	while (1)
	{
		/* your game of life code goes here  */
        process_generation(lifeA, lifeRows, lifeCols, lifeB, edge);
        // swap matrixes
        unsigned char **temp = lifeA;
        lifeA = lifeB;
        lifeB = temp;

		/* change the  modulus value to slow the rendering */
		 /* if (SDL_GetTicks() % 1 == 0) */
		 if (SDL_GetTicks() % 10 == 0)
            sdl_render_life(&sdl_info, lifeA);
			//sdl_test(&sdl_info, m, n);

                 /* Poll for events, and handle the ones we care about.
                  * You can click the X button to close the window
                  */
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                        /* If escape is pressed, return (and thus, quit) */
				if (event.key.keysym.sym == SDLK_ESCAPE) {
                    free_life(&lifeA, lifeRows, lifeCols);
                    free_life(&lifeB, lifeRows, lifeCols);
					return 0;
                }
				break;
			case SDL_QUIT:
            {
                free_life(&lifeA, lifeRows, lifeCols);
                free_life(&lifeB, lifeRows, lifeCols);
				return(0);
            }
			}
		}
	}
    free_life(&lifeA, lifeRows, lifeCols);
    free_life(&lifeB, lifeRows, lifeCols);
	return 0;
}
