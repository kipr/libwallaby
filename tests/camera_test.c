/*
 * camera_test.c
 *
 *  Created on: Jan 7, 2015
 *      Author: Nafis Zaman
 */

#include <stdio.h>

#include "wallaby/wallaby.h"

int main(int argc, char ** argv)
{
  printf("Opening camera... ");
  int ret = camera_open_at_res(MED_RES);
  printf("Success? %d\n", ret);
  printf("\n");
  
  printf("Waiting 3 seconds...\n");
  msleep(3000L);
  
  /*printf("Loading test1...\n");
  camera_load_config("test1");
  msleep(2000L);
  printf("Loaded\n");
  
  printf("Loading test1...\n");
  camera_load_config("test2");
  msleep(2000L);
  printf("Loaded\n");*/
  
  printf("Camera width: %d\n", get_camera_width());
  printf("Camera height: %d\n", get_camera_height());
  printf("\n");
  
  int i;
  for(i = 1; i <= 100; ++i) {
    //printf("SAMPLE %d\n", i);
    //printf("Updating camera... ");
    ret = camera_update();
    //printf("Success? %d\n", ret);
    
    /*const int count = get_object_count(0);
    const int conf = get_object_confidence(0, 0);
    const int area = get_object_area(0, 0);
    const int x = get_object_centroid_x(0, 0);
    const int y = get_object_centroid_y(0, 0);
    
    printf("Object count: %d\n", count);
    if(count > 0) {
      printf("Largest object confidence: %d\n", conf);
      printf("Largest object area: %d\n", area);
      printf("Largest object center: (%d, %d)\n", x, y);
    }
    printf("\n");*/
    
    msleep(1000L);
  }
  
  printf("Closing camera...\n");
  camera_close();
  
  printf("Done!\n");
  
  return 0;
}