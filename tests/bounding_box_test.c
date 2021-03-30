// Copyright(c) KIPR, 2019
// Demo for using the graphics API to display the camera image 
//   along as many as 5 bounding boxes for blobs on color channels.  
// If a bounding box is touched, the program reports its channel and number.
//
// Initial version: 6/11/2014 - cnw
// Revision: 9/3/2019 - cnw
//    1. modified and modularized for Wombat KRC

#include <stdio.h>
#include <kipr/graphics.h>
#include <kipr/graphics_characters.h>
#include <kipr/camera.h>
#include <kipr/button.h>
// functions to be included in Wombat KRC library
void display_printf(int col, int row, char *t, ...);
void display_clear();

// current camera resolution supported by software
#define CAMCOLS 160
#define CAMROWS 120
#define RES 0  // LOW_RES=0, MED_RES=1, HIGH_RES=2 (only LOW_RES supported according to graphics.h)
#define SM 3   // Screen size multiplier (base size, double, or triple)
void set_cam_image(); // installs camera image in graphics window
void install_bboxes(int rgb[][3], rectangle bbcx[][5], int nx[]); // adds up to 5 bounding boxes 
                                                // to graphics window, color r,g,b
int main()
  {
#ifdef WITH_VISION_SUPPORT
    int n[3], b, c, cc, tf, row, col, rgb[3][3]={{255,0,0},{0,255,0},{0,0,255}}; // channel color for boxes
    rectangle bbcx [3][5]; // array of rectangles to hold up to 5 bounding boxes for up to 3 channels
    camera_open();         // open camera 
    cc = get_channel_count();  // n[c] counts number of "large" blobs on channel c
    graphics_open(SM*CAMCOLS, SM*CAMROWS);  // camera window starts in middle of the screen, move as desired
    display_clear();   // get rid of any messages from camera_open()
    display_printf(0,4,"Press B"); display_printf(0,5,"to end");
    while(b_button()==0){    // B button used for exit
      camera_update();       // get new camera data
      set_cam_image();       // paint camera image on graphics window
	  install_bboxes(rgb, bbcx, n);   // draw up to 5 bounding boxes on window for each channel
      graphics_update();     // show current camera image with bounding boxes
      if (get_mouse_left_button()) { // has user tapped the screen?
        get_mouse_position(&col, &row);
        tf=0;
        display_printf(0,0,"Last tap  ");
        for(c=0;c<cc;c++) {
          for(b=0; b<n[c]; b++) {
            if ((col>=SM*bbcx[c][b].ulx) && (col<=(SM*bbcx[c][b].ulx+SM*bbcx[c][b].width)) &&
                (row>=SM*bbcx[c][b].uly) && (row<=(SM*bbcx[c][b].uly+SM*bbcx[c][b].height))) {
              display_printf(0,1,"[%d] blob %d   ",c,b); // report blob tapped
              tf=1;  // tap flag
              c=3; break; // don't check for more blobs
            }
          }
        }
        if (tf==0) {
          display_printf(0,1,"not blob         ");
        }
      }
    }
    // clean things up and exit
    camera_close();
    graphics_close();
#else
    printf("This platform does not support camera");
#endif

    return 0;
}

#ifdef WITH_VISION_SUPPORT

// display the camera CAMCOLSxCAMROWS pixel image in a graphics window
void set_cam_image()
{
  const unsigned char *img=get_camera_frame();
  // Frame data is in BGR 888 pixel format: 3 bytes per pixel; each
  // character is the 8-bit (unsigned) integer value for each BGR color
  int row, col, i, j, rgb[3]={2,1,0}; // array used to convert BGR to RGB 
  for(row=0;row<CAMROWS;row++) // process frame to graphics window
    for(col=0;col<CAMCOLS;col++)
      for(i=0;i<SM;i++) // paint pixel in SM x SM square
        for(j=0;j<SM;j++) {
          graphics_pixel(SM*col+j,SM*row+i, 
          img[3*(CAMCOLS*row+col)+rgb[0]],img[3*(CAMCOLS*row+col)+rgb[1]],
                 img[3*(CAMCOLS*row+col)+rgb[2]]);
        }
}
void install_bboxes(int rgb[][3], rectangle bbcx[][5], int nx[]) 
{
   int b, n, c, cc; 
   rectangle bb;
   cc = get_channel_count();    // assumes at least one channel configuration
   for (c=0; c<cc; c++) {       // draw boxes for each channel
     nx[c]=get_object_count(c); // number of blobs for channel c
     nx[c]=nx[c]<5 ? nx[c] : 5; // limit is the 5 largest blobs
     if (nx[c] == 0) break;     // no blobs for this channel
     n=nx[c];
     for(b=0; b<n; b++){        // there is at least one blob so get its data
       bb = get_object_bbox(c,b);
       if (bb.width < 6 || bb.height < 6) { // channel has no more large blobs
          nx[c]=b; n=b;
          break; 
       }
       if (n>0) { // draw rectangle for the current blob
         graphics_rectangle(SM*bb.ulx,SM*bb.uly,(SM*bb.ulx+SM*bb.width),
                           (SM*bb.uly+SM*bb.height), rgb[c][0], rgb[c][1], rgb[c][2]);
         bbcx[c][b].ulx=bb.ulx; bbcx[c][b].uly=bb.uly; // record blob data for mouse check
         bbcx[c][b].width=bb.width; bbcx[c][b].height=bb.height;
         g_printInt(b,1,(SM*bb.ulx+SM*bb.width-10),
                           (SM*bb.uly+SM*bb.height-15), rgb[c][0], rgb[c][1], rgb[c][2], 1);
       }
     }
   }
   return;
}

#endif
