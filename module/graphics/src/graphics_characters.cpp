/*
 * graphics_characters.cpp
 *
 *  Created on: Feb 10, 2016
 *      Author: kipr
 */

/* Includes the Standard IO Library */


#include <string.h>
#include <math.h>


#include "kipr/graphics/graphics_characters.h"
#include "kipr/graphics/graphics.h"

/* These are functions for adding alphameric characters (caps only) to graphics windows.
   The functions were extracted from a graphing package written by D.Miller Oct 2008.
   Character generation capabilities were extended from 7-segment to 16-segment and
   enhanced to include basic punctuation characters, plus size and bold-face options
   by C.Winton Oct 2008.
   */

// **************** Constants ******************

// The number of segments in a character display
/* Segment arrangement
	 --0- -1--     ---18----     --20 21--     ---- ----
	|\   |   /|	  |\  18   /|	|\20   21/|	  ||   |   /|
	7 8  9 10 2	  | \  |  / |	|24  |  26|	  ||\ 29  / |
	|   \|/   |	  |   17/   |	24   |/  26	  || 29|/   |
	 -15- -11-	   ---17----	 ---- ----	   |--- ----
	|   /|\   |	  |   /|\   |	25  /|\  27	  |2  /|\   |
	6 14 1 12 3	  |  / |  \ |	|25  |  27|	  |8 / |  \ |
	|/   3   \|	  |/  16   \|	|/22   23\|	  ||   |   \|
 	 --5- -4--	   ---16----	 --22 23--	   ---- ----
                       /
                	  / (19)
*/
/*
#define NUMSEG 30
// SEGL specifies the length of a vertical segment in pixels
#define SEGL 5
// SEGSP is the spacing of characters
#define SEGSP 7
// The width in pixels of the window
#ifndef GXWINDOW
  #define GXWINDOW 700
#endif
// The height of the window
#ifndef GYWINDOW
  #define GYWINDOW 500
#endif
// **************** Function Prototypes ******************
// Based on 16 segment characters: draws a segment for each s value that gets a 1 and a rectangle if the value is 2;
// the upper left corner starts at x,y; the color is defined by the vales of rgb; multiplier size resizes the character;
// there are added segments to provide for . : , ; and other non alphameric characters
// arrows (l,r,u,d) are specified by numbers 11 ,12, 13, 14

void graphics_segment_display(int s[], int x, int y, int r, int g, int b, float size);
// uses graphics_segment_display to draw a 16 segment character (alphameric - caps only)
void graphics_print_character(int n, int x, int y, int r, int g, int b, float size);
//  prints out a text string in 16 segment characters
void graphics_print_string(char s[], int x, int y, int r, int g, int b, float size);
// prints out an integer in 16 segment characters. minNumDigits is normally 0, but can be larger if leading 0's are desired
int graphics_print_int(int n, int minNumDigits, int x, int y, int r, int g, int b, float size);
// prints out a float. numDigits is the number of digits to right of decimal to be printed
void graphics_print_float(float n, int numDigits, int x, int y, int r, int g, int b, float size);
*/

// bold faces print; usage: BOLD g_print<fn>;
int __bold=0;
#ifndef _graphics_characters_
#define BOLD __bold=1;
#define NOBOLD __bold=0;
#endif


void graphics_print_string(char s[],int x, int y, int r, int g, int b, float size)
{
	int i,l;
	l=strlen(s);
	for(i=0;i<l;i++) {
		graphics_print_character(s[i],x+i*size*SEGSP,y,r,g,b,size);
	}
}

int graphics_print_int(int n, int minNumDigits, int x, int y, int r, int g, int b, float size)
{
	int i,d, count=0, numDigits;
	if(n<0){
		graphics_print_character('-',x,y,r,g,b,size);
		n = -n;
		x = x + SEGSP*size;
		count++;
	}
	numDigits = ceil(log10(n+0.000001));//get the right number of digits for 1,10,100, etc
	if(numDigits > minNumDigits) minNumDigits = numDigits;
	for(i = minNumDigits-1; i>= 0; i--){
		if(i < (minNumDigits - numDigits)) {
			d = 0; //print a leading 0
			minNumDigits--;
		}
		else{
			d=n%10;
			n=n/10;
		}
		graphics_print_character(d,i*SEGSP*size + x, y, r,g,b,size);
		count++;
	}
	return(count);
}

void graphics_print_float(float n, int numDigits, int x, int y, int r, int g, int b, float size)
{
	int whole, frac, offset;
	whole = (int) n;
	frac = (int)(fabs(n - whole) * pow(10.000001,numDigits)); // corrective factor for float->int conversions
	//	printf(" n %f, w %d f %d n-w %f\n",n,whole,frac,n-whole);
	if(whole == 0 && n <0.0){
		graphics_print_character('-',x,y,r,g,b,size);
		graphics_print_int(whole,1,x+SEGL,y,r,g,b,size);
		offset=2;
	}
	else{
		offset = graphics_print_int(whole,1,x,y,r,g,b,size);
	}
	graphics_print_character('.',x + offset*SEGSP*size,y, r,g,b,size);
	graphics_print_int(frac,numDigits,x + (offset+1)*SEGSP*size, y,r,g,b,size);
}

void graphics_print_character(int n, int x, int y, int r, int g, int b, float size)
{
	int d[NUMSEG]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	switch(n){
		case 0: case'0':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[6]=d[7]=d[10]=d[14]=1;
			break;
		}
		case 1: case'1':{
			d[2]=d[3]=1;
			break;
		}
		case 2: case'2':{
			d[0]=d[1]=d[2]=d[4]=d[5]=d[11]=d[14]=1;
			break;
		}
		case 3: case'3':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[11]=1;
			break;
		}
		case 4: case'4':{
			d[2]=d[3]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 5: case'5':{
			d[0]=d[1]=d[4]=d[5]=d[7]=d[12]=d[15]=1;
			break;
		}
		case 6: case'6':{
			d[0]=d[3]=d[4]=d[5]=d[6]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 7: case'7':{
			d[0]=d[1]=d[10]=d[14]=1;
			break;
		}
		case 8: case'8':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[6]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 9: case'9':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[7]=d[11]=d[15]=1;
			break;
		}
		case '.':{
			d[16]=2;
			break;
		}
		case ':':{
			d[16]=d[17]=2;
			break;
		}
		case ',':{
			d[16]=2; d[19]=1;
			break;
		}
		case ';':{
			d[16]=d[17]=2; d[19]=1;
			break;
		}
		case '-':{
			d[11]=d[15]=1;
			break;
		}
		case '+':{
			d[9]=d[11]=d[13]=d[15]=1;
			break;
		}
		case '*':{
			d[8]=d[10]=d[11]=d[12]=d[14]=d[15]=1;
			break;
		}
		case '/':{
			d[10]=d[14]=1;
			break;
		}
		case '\\':{
			d[8]=d[12]=1;
			break;
		}
		case '_': {
			d[4]=d[5]=1;
			}
		case ' ':{
			break;
		}
		case 'A': case'a':{
			d[0]=d[1]=d[2]=d[3]=d[6]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 'B': case'b':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[11]=d[28]=1;
			break;
		}
		case 'C': case'c':{
			d[0]=d[1]=d[4]=d[5]=d[6]=d[7]=1;
			break;
		}
		case 'D': case'd':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[28]=1;
			break;
		}
		case 'E': case'e':{
			d[0]=d[1]=d[4]=d[5]=d[6]=d[7]=d[15]=1;
			break;
		}
		case 'F': case'f':{
			d[0]=d[1]=d[6]=d[7]=d[15]=1;
			break;
		}
		case 'G': case'g':{
			d[0]=d[1]=d[3]=d[4]=d[5]=d[6]=d[7]=d[11]=1;
			break;
		}
		case 'H': case'h':{
			d[2]=d[3]=d[6]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 'I': case'i':{
			d[0]=d[1]=d[4]=d[5]=d[9]=d[13]=1;
			break;
		}
		case 'J': case'j':{
			d[2]=d[3]=d[4]=d[5]=d[6]=1;
			break;
		}
		case 'K': case'k':{
			d[6]=d[7]=d[10]=d[12]=d[15]=1;
			break;
		}
		case 'L': case'l':{
			d[4]=d[5]=d[6]=d[7]=1;
			break;
		}
		case 'M': case'm':{
			d[2]=d[3]=d[6]=d[7]=d[8]=d[10]=1;
			break;
		}
		case 'N': case'n':{
			d[2]=d[3]=d[6]=d[7]=d[8]=d[12]=1;
			break;
		}
		case 'O': case'o':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[6]=d[7]=1;
			break;
		}
		case 'P': case'p':{
			d[0]=d[1]=d[2]=d[6]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 'Q': case'q':{
			d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=d[6]=d[7]=d[12]=1;
			break;
		}
		case 'R': case'r':{
			d[0]=d[1]=d[2]=d[6]=d[7]=d[11]=d[12]=d[15]=1;
			break;
		}
		case 'S': case's':{
			d[0]=d[1]=d[3]=d[4]=d[5]=d[7]=d[11]=d[15]=1;
			break;
		}
		case 'T': case't':{
			d[0]=d[1]=d[9]=d[13]=1;
			break;
		}
		case 'U': case'u':{
			d[2]=d[3]=d[4]=d[5]=d[6]=d[7]=1;
			break;
		}
		case 'V': {
			d[6]=d[7]=d[10]=d[14]=1;
			break;
		}
		case 'W': case'w':{
			d[2]=d[3]=d[6]=d[7]=d[12]=d[14]=1;
			break;
		}
		case 'X': case'x':{
			d[8]=d[10]=d[12]=d[14]=1;
			break;
		}
		case 'Y': case'y':{
			d[8]=d[10]=d[13]=1;
			break;
		}
		case 'Z': case'z':{
			d[0]=d[1]=d[4]=d[5]=d[10]=d[14]=1;
			break;
		}
		case '~': {
			d[0]=d[1]=1;
			break;
			}
		case '@': {
			d[1]=d[2]=d[3]=d[4]=d[11]=d[13]=1;
			break;
			}
		case '&': { // poor
			d[0]=d[5]=d[6]=d[7]=d[9]=d[12]=d[13]=d[15]=1;
			break;
		}
		case '%': {
			d[10]=d[14]=1; d[16]=d[18]=2;
			break;
		}
		case '$': {
			d[0]=d[1]=d[3]=d[4]=d[5]=d[7]=d[9]=d[11]=d[13]=d[15]=1;
			break;
		}
		case '[': case '(': case '{': {
			d[0]=d[5]=d[6]=d[7]=1;
			break;
		}
		case ']': case ')': case '}': {
			d[1]=d[2]=d[3]=d[4]=1;
			break;
		}
		case '!':{
			d[9]=1; d[16]=2;
		break;
		}
		case '?': {
			d[0]=d[1]=d[2]=d[11]=d[13]=1; d[16]=2;
			break;
		}
		case '|': {
			d[9]=d[13]=1;
			break;
		}
		case '\'': case '\"':{
			d[18]=2; d[29]=1;
			break;
		}
		case '>': {
			d[8]=d[14]=1;
			break;
		}
		case '<': {
			d[10]=d[12]=1;
			break;
		}
		case '=': {
			d[4]=d[5]=d[11]=d[15]=1;
			break;
		}
		case '^': {
			d[12]=d[14]=1;
			break;
		}
		case 'v': {
			d[8]=d[10]=1;
			break;
		}
		case 11: {  // arrow left
			d[11]=d[15]=d[24]=d[25]=1;
			break;
		}
		case 12: {  // arrow right
			d[11]=d[15]= d[26]=d[27]=1;
			break;
		}
		case 13: {  // arrow up
			d[9]=d[13]=d[20]=d[21]=1;
			break;
		}
		case 14: {  // arrow down
			d[9]=d[13]=d[22]=d[23]=1;
			break;
		}
		default: {
			d[0]=d[1]=d[2]=d[3]=d[4]=d[8]=d[10]=d[12]=d[14]=1;
			break;
		}
	}
	graphics_segment_display(d,x,y,r,g,b,size);
}
/* Segment arrangement
	 --0- -1--     ---18----     --20 21--     ---- ----
	|\   |   /|	  |\  18   /|	|\20   21/|	  ||   |   /|
	7 8  9 10 2	  | \  |  / |	|24  |  26|	  ||\ 29  / |
	|   \|/   |	  |   17/   |	24   |/  26	  || 29|/   |
	 -15- -11-	   ---17----	 ---- ----	   |--- ----
	|   /|\   |	  |   /|\   |	25  /|\  27	  |2  /|\   |
	6 14 1 12 3	  |  / |  \ |	|25  |  27|	  |8 / |  \ |
	|/   3   \|	  |/  16   \|	|/22   23\|	  ||   |   \|
 	 --5- -4--	   ---16----	 --22 23--	   ---- ----
                       /
                	  / (19)
*/
void graphics_segment_display(int s[], int x, int y, int r, int g, int b, float size)
{
	int segmap[NUMSEG][2][2]={
		{{0,0},{2,0}},  // 0
		{{2,0},{4,0}},  // 1
		{{4,0},{4,4}},  // 2
		{{4,4},{4,8}},  // 3
		{{4,8},{2,8}},  // 4
		{{2,8},{0,8}},  // 5
		{{0,8},{0,4}},  // 6
		{{0,4},{0,0}},  // 7
		{{2,4},{0,0}},  // 8
		{{2,4},{2,0}},  // 9
		{{2,4},{4,0}},  // 10
		{{2,4},{4,4}},  // 11
		{{2,4},{4,8}},  // 12
		{{2,4},{2,8}},  // 13
		{{2,4},{0,8}},  // 14
		{{2,4},{0,4}},  // 15
		{{2,7},{3,8}},  // 16 (for lower dot)
		{{2,3},{3,4}},  // 17 (for mid dot)
		{{2,0},{3,1}},  // 18 (for upper dot)
		{{2,10},{3,9}}, // 19 (for comma, semi-colon descender)
		{{0,2},{2,0}},  // 20 (/ for arrow up)
		{{2,0},{4,2}},  // 21 (\ for arrow up)
		{{2,8},{0,6}},  // 22 (\ for arrow down)
		{{2,8},{4,6}},  // 23 (/ for arrow down)
		{{0,4},{2,2}},  // 24 (/ for arrow left)
		{{0,4},{2,6}},  // 25 (\ for arrow left)
		{{4,4},{2,2}},  // 26 (\ for arrow right)
		{{4,4},{2,6}},  // 27 (/ for arrow right)
        {{1,0},{1,8}},  // 28 (improved B and D)
        {{2,3},{3,2}},	// 29 (apostrophe descender)
	};
	int i;
	for(i=0;i<NUMSEG;i++){
		if(s[i]==1) {
			graphics_line(x+size*segmap[i][0][0],y+size*segmap[i][0][1],
				x+size*segmap[i][1][0],y+size*segmap[i][1][1],r,g,b);
			if (__bold==1)
				graphics_line(1+x+size*segmap[i][0][0],1+y+size*segmap[i][0][1],
					1+x+size*segmap[i][1][0],1+y+size*segmap[i][1][1],r,g,b);
		}
		else if(s[i]==2)
			graphics_rectangle(x+size*segmap[i][0][0],y+size*segmap[i][0][1],
				x+size*segmap[i][1][0],y+size*segmap[i][1][1],r,g,b);
	}
}

