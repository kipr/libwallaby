/*
 * analog_test.c
 *
 *  Created on: Nov 4, 2015
 *      Author: Joshua Southerland
 */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <wallaby/wallaby.h>

int main(int argc, char const *argv[]) {

#ifdef ARUCO
  printf("Setting the dictionary to 1\n");
  set_dictionary(1);
  printf("Getting the pose for id 5 (should be 0 0 0 0 0 0 )\n");
  double *pose = get_marker_pose(5);
  printf("Pose %f %f %f %f %f %f\n", pose[0], pose[1], pose[2], pose[3],
         pose[4], pose[5]);
  // printf("Getting all the markers in view\n");
  // int* ids = get_markers_in_view();
  // printf("Markers in view: ");
  printf("Checking if the camera can see marker 0");
  bool canSeeZero = can_see_marker(0);
  printf("Can see marker zero ");
  printf(canSeeZero ? "true\n" : "false\n");
// printf("Running the calibration\n");
// bool calibrated = camera_calibrate();
// printf(calibrated ? "true\n" : "false\n");
#else
  printf("This platform does not support ArUco");
#endif

  return 0;
}
