// Copyright(C) KIPR, 2019
//  cnw - 9/8/2019
#include <stdio.h>
#include <kipr/camera.h>
#include <kipr/graphics.h>
#include <kipr/util.h>
#include <kipr/button.h>
#define CAMCOLS 160
#define CAMROWS 120
#define RES 0  // LOW_RES=0, MED_RES=1, HIGH_RES=2 (only LOW_RES supported according to graphics.h)
#define SM 1
int main() // assumes channel 2 is for QR codes
{
#ifdef WITH_VISION_SUPPORT
    const char *cp;
    const unsigned char *img;
    int i, j, n, cc, row, col;
    int rgb[3]={2,1,0}; // array used to convert BGR to RGB
    rectangle bb;
    cc = get_channel_count();  
    printf("%d channels present, B to quit\n",cc);
    graphics_open(SM*CAMCOLS, SM*CAMROWS);     
	camera_open();
    // display the camera CAMCOLSxCAMROWS pixel image in a graphics window
    camera_update();
    graphics_update();
    while(b_button()==0) {
      img=get_camera_frame();  // get new frame
      for(row=0;row<CAMROWS;row++) // process frame to graphics window
        for(col=0;col<CAMCOLS;col++)
          for(i=0;i<SM;i++) // paint pixel in SM x SM square
            for(j=0;j<SM;j++) {
              graphics_pixel(SM*col+j,SM*row+i, 
              img[3*(CAMCOLS*row+col)+rgb[0]],img[3*(CAMCOLS*row+col)+rgb[1]],
                     img[3*(CAMCOLS*row+col)+rgb[2]]);
            }
      if (0!=get_object_count(2)) {
          bb = get_object_bbox(2,0);  // QR code image
          graphics_rectangle(SM*bb.ulx,SM*bb.uly,(SM*bb.ulx+SM*bb.width),
                           (SM*bb.uly+SM*bb.height), 255, 0, 0);
          n = get_object_data_length(2,0);  // channel 2, object 0
          cp = get_object_data(2,0);
    	  for (i=0;i<n;i++) printf ("%c",cp[i]);
    	  printf("\n");
      }
      graphics_update();  // display the window
      camera_update();
    }
    camera_close();
    printf("done\n");
#else
    printf("This platform does not support camera");
#endif

    return 0;
}
