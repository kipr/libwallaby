#include <stdio.h>
#include <kipr/graphics.h>
#include <kipr/util.h>
#include <kipr/button.h>
#include "kipr/countdown_timer.h"
#define CORNERS123 int corner1[2]={90,50}; int corner2[2]={200,50}; int corner3[2]={310,50}; 
#define CORNER2 int corner2[2]={200,50};
#define HEADER(x) g_printString("  BOTBALL 120 SECOND COUNTDOWN", 0, 5, x, 1); \
                  g_printString("  ELAPSED ", 0, 25, x, 1); 
// global managed by countdown
int __e__ = 0; // elapsed time
int main()
{
    int i;
    printf("Test start\n");
    graphics_open(FULL_SCREEN);
    graphics_fill(BLUE);
    HEADER(WHITE)
    ELAPSED(255,255,255)
    countdown_sd(5, 1, BLUE, YELLOW);
    graphics_fill(GREEN);
    HEADER(BLACK)
    countdown(120, 106, GREEN, BLUE);
    graphics_fill(LTBLUE);
    HEADER(BLACK)
	countdown(105, 11, LTBLUE, RED);
    graphics_fill(LTORANGE);
    HEADER(BLACK)
	countdown(10, 6, LTORANGE, BLUE);
    graphics_fill(RED);
    HEADER(WHITE)
    countdown(5, 0, RED, YELLOW);
    msleep(1000);
    for(i=0;i<10;i++) { // flash 000
       showtriple(0,RED); msleep(100); 
       showtriple(0,YELLOW); msleep(100);
    }
	msleep(1000); // pause display
    graphics_close(); // clean up
    printf("Test done\n");
    return 0;
}

//#include "kipr/countdown_timer.h"
void draw_seg(int strt_col, int strt_row, int end_col, int end_row, int seg_width, int r, int g, int b) 
{
  int i;
  for (i=-seg_width; i<=seg_width; i++) graphics_line(strt_col, strt_row+i, end_col, end_row+i, r,g,b);
  for (i=-seg_width; i<=seg_width; i++) graphics_line(strt_col+i, strt_row, end_col+i, end_row, r,g,b);  
  graphics_circle_fill(strt_col, strt_row, seg_width, r,g,b);
  graphics_circle_fill(end_col, end_row, seg_width, r,g,b);
}
// ul_corner is (col,row)
void seven_seg(int ul_corner[2], int seg_length, int segs[12], int r, int g, int b) 
{
    int ulc=ul_corner[0], ulr=ul_corner[1];
    if (segs[0]) { // a
        draw_seg(ulc, ulr, ulc+seg_length, ulr, 5, r,g,b);
    }
    if (segs[9]) { // short a
        draw_seg(ulc, ulr, ulc+seg_length-10, ulr, 5, r,g,b);
    }
    if (segs[1]) { // b
        draw_seg(ulc+seg_length+3, ulr+4, ulc+seg_length+3, ulr+seg_length-4, 5, r,g,b);
    }
    if (segs[7]) { // long b
        draw_seg(ulc+seg_length+3, ulr, ulc+seg_length+3, ulr+seg_length-4, 5, r,g,b);
    }
    if (segs[2]) { // c
        draw_seg(ulc+seg_length+3, ulr+seg_length+4, ulc+seg_length+3, ulr+2*seg_length-4, 5, r,g,b);
    }
     if (segs[8]) { // long c
        draw_seg(ulc+seg_length+3, ulr+seg_length+4, ulc+seg_length+3, ulr+2*seg_length, 5, r,g,b);
    }
    if (segs[3]) { // d
        draw_seg(ulc, ulr+2*seg_length, ulc+seg_length, ulr+2*seg_length, 5, r,g,b);
    }
    if (segs[10]) { // short d
        draw_seg(ulc+10, ulr+2*seg_length, ulc+seg_length, ulr+2*seg_length, 5, r,g,b);
    }    
    if (segs[4]) { // e
        draw_seg(ulc-3, ulr+seg_length+4, ulc-3, ulr+2*seg_length-4, 5, r,g,b);
    }
    if (segs[5]) { // f
        draw_seg(ulc-3, ulr+5, ulc-3, ulr+seg_length-4, 5, r,g,b);
    }
    if (segs[6]) { // g
        draw_seg(ulc, ulr+seg_length, ulc+seg_length, ulr+seg_length, 5, r,g,b);        
    }
    if (segs[11]) { // short g
        draw_seg(ulc+10, ulr+seg_length, ulc+seg_length, ulr+seg_length, 5, r,g,b);        
    }    
}

// left most column,row of bounding box, height of bounding box, digit, segment width 
// 
void draw_digit(int col_row[2], int seg_length, int d, int r, int g, int b) // return width of digit 
{
   int segs[12]={0,0,0,0,0,0,0,0,0,0,0,0};
   switch (d) {
     case 1: 
       segs[7]=1; segs[8]=1; // long b, long c   
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 2: 
       segs[0]=1; segs[1]=1; segs[3]=1; segs[4]=1; segs[6]=1; // short a,b,d,e,g  
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 3: 
       segs[0]=1; segs[1]=1; segs[2]=1; segs[3]=1; segs[11]=1; // a,b,c,d,short g   
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 4: 
       segs[7]=1; segs[8]=1; segs[5]=1; segs[6]=1; // b,c,f,g   
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 5: 
       segs[9]=1; segs[2]=1; segs[3]=1; segs[5]=1; segs[6]=1; // short a,c,d,f,g     
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 6: 
       segs[9]=1; segs[2]=1; segs[3]=1; segs[4]=1; segs[5]=1; segs[6]=1; // short a,c,d,e,f,g      
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 7: 
       segs[0]=1; segs[1]=1; segs[8]=1;  // a,b,c     
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 8: 
       segs[0]=1; segs[1]=1; segs[2]=1; segs[3]=1; segs[4]=1; segs[5]=1; segs[6]=1; // a,b,c,d,e,f,g   
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;
     case 9: 
       segs[0]=1; segs[1]=1; segs[2]=1; segs[10]=1; segs[5]=1; segs[6]=1; // a,b,c,short d,f,g             
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;  
     case 0: 
       segs[0]=1; segs[1]=1; segs[2]=1; segs[3]=1; segs[4]=1; segs[5]=1; // a,b,c,d,e,f   
       seven_seg(col_row, seg_length, segs, r,g,b);    
       break;           
     default:
       break;
   }
}


int countdown(int strt, int fin, int br, int bg, int bb, int dr, int dg, int db)
{
	int i, j, d1, d2, d3; // segments 5 pixels wide
    double s;
    CORNERS123
	while(black_button()); // debounce
	for(i=strt; i>=fin; i--) {
        s = seconds();
		d1=i/100; d2=(i%100)/10; d3=i%10;
        if (d1>0) draw_digit(corner1, 75, d1, dr, dg, db);
		if (d1>0 || d2>0) draw_digit(corner2, 75, d2, dr, dg, db);
		draw_digit(corner3, 75, d3, dr, dg, db);
        ELAPSED(0,0,0);
		graphics_update(); // show digits
        if (d1>0) draw_digit(corner1, 75, d1, br, bg, bb); // erase digits
		if (d1>0 || d2>0) draw_digit(corner2, 75, d2, br, bg, bb);
		draw_digit(corner3, 75, d3, br, bg, bb);
		for (j=0;j<3;j++) { // pause 3/4 second or quit
			if(BCHK) return 1;
			msleep(250);
		}
        msleep(1000-1000*(seconds()-s)); // consume rest of the second
        ELAPSED(br, bg, bb) // erase current
        __e__+=1;
	}
	return 0;	
}

int countdown_sd(int strt, int fin, int br, int bg, int bb, int dr, int dg, int db) // single digit version
{
	int i, j/*, d1, d2*/, d3;
    double s;
    CORNER2
	while(BCHK); // debounce
	for(i=strt; i>=fin; i--) {
        s = seconds();
		d3=i%10;
		draw_digit(corner2, 75, d3, dr, dg, db);
		graphics_update(); // show digit
        draw_digit(corner2, 75, d3, br, bg, bb); // erase digit
		for (j=0; j<3; j++) { // pause 3/4 second or quit
			if(black_button()) return 1;
			msleep(250);
		}
        msleep(1000-1000*(seconds()-s)); // consume rest of the second
	}
	return 0;	
}

void showtriple(int i, int r, int g, int b) 
{
    CORNERS123
	draw_digit(corner1, 75, 0, r, g, b);
	draw_digit(corner2, 75, 0, r, g, b);
	draw_digit(corner3, 75, 0, r, g, b);
	graphics_update();
}

