/*
 * aruco_c.cpp
 *
 *  Created on: April 10, 2017
 *      Author: Ryan Owens
 */

#include "wallaby/aruco.hpp"
#include "wallaby/aruco.h"

using namespace aruco;

void set_dictionary(int dictionaryId)
{
  // Actually creates the instance
  Aruco* dev = Aruco::getInstance();
}

double* get_marker_pose(int markerId)
{
  // TODO proper interface...
  Aruco* dev = Aruco::getInstance();
  std::vector<double> pose = dev->getPose(markerId);
  return pose.data();
}

int* get_markers_in_view()
{
  // TODO proper interface...
  Aruco* dev = Aruco::getInstance();
  std::vector<int> ids = dev->arucoMarkersInView();
  return ids.data();
}

signed short can_see_marker(int marker)
{
  // TODO proper interface...
  Aruco* dev = Aruco::getInstance();
  return dev->arucoMarkerInView(marker);
}


// signed short camera_calibrate()
// {
//   // TODO Needs to be completely different but without knowing the
//   // desired interface...
//   Aruco* dev = Aruco::getInstance();
//   return dev->calibrate();
// }
