// copyright (c), KIPR 2019
// countdown timer
//     Charles Winton: 8/17/2019
// Elements have been extracted from prior table programs 

// standard libraries needed to compile 
#include <stdio.h>
#include <stdlib.h> // for rand, srand
#include <math.h>   // for abs
#include <kipr/graphics.h>
#include <kipr/graphics_characters.h>
#include <kipr/util.h>

#define RED 255,0,0
#define MAROON 128,0,0
#define CORAL 240,128,128
#define PINK 255,105,180
#define ROSE 255,228,225
#define ORANGE 255,153,51
#define MAGENTA 255,0,255
#define FUCHSIA 255,0,255
#define PURPLE 128,0,128
#define VIOLET 138,43,226
#define GOLD 255,215,0
#define YELLOW 255,255,0
#define LIME 0,255,0
#define GREEN 0,255,0
#define OLIVE 128,128,0
#define CYAN 0,255,255
#define AQUA 0,255,255
#define TEAL 0,128,128
#define BLUE 0,0,255
#define NAVY 0,0,128
#define ALMOND 255,235,205
#define LTBROWN 255,229,220
#define BROWN 139,69,19
#define SIENNA 160,82,45
#define WHITE 255,255,255
#define BLACK 0,0,0
#define DKGRAY 84,84,84
#define GRAY 128,128,128
#define SHADOW 170,170,170
#define SILVER 192,192,192
#define LTGRAY 211,211,211
#define LTORANGE 255,229,204
#define LTBLUE 204,255,255
// buttons debounce/check for press
#define BCHK a_button() || b_button() || c_button() || black_button()
// screen information
#define MAX_ROW 247
#define MAX_COL 471
#define FULL_SCREEN MAX_COL,MAX_ROW
extern int __e__; // elapsed time
#define ELAPSED(r,g,b) g_printInt(__e__ , 3, 70, 25, r,g,b, 1);
// digital ports used for table lights
void table_lights(int lports[], int pcnt, int onoff); // table lights on/off
void setup(); // light calibration (includes time-out)
// void blinking_light(); // blinking light image shown during light calibration period 2016
// functions used to determine shelf positions
// void get_shelf_positions(); // 2014
// void get_bgal_pod_positions(); // 2015
// void pick2(int* x, int* y); // set pair of random values from 0-6 at least 2 apart
// functions for 7-segment display (strt time, finish time, box rgb, digit rgb, margin, displacement)
int countdown(int strt, int fin, int br, int bg, int bb, int dr, int dg, int db); // basic countdown
int blinkdown(int strt, int fin, int br, int bg, int bb, int dr, int dg, int db, int lports[], int pcnt, float s); // countdown blinking lights version
int countdown_sd(int strt, int fin, int br, int bg, int bb, int dr, int dg, int db); // countdown centering a single digit
void showtriple(int i, int r, int g, int b) ; // show i in all 3 digit positions
// splash screen
void SplashBG(int w, int h);
void draw_bg(int x, int y, double pct);
void clear_bg(int x, int y, double pct);
#define POINT struct xy
struct xy {
	int x;
	int y;
} ;
void docircle(POINT ctr, int rad, int r, int g, int b);
void dorectangle(POINT ul, POINT lr, int r, int g, int b);
void dotriangle(POINT p1, POINT p2, POINT p3, int r, int g, int b);
void set_scale_trans(int x, int y, double pct);
void draw_digit(int col_row[2], int seg_length, int d, int r, int g, int b); // return width of digit 
