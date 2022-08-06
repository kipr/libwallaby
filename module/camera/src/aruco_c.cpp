#include "kipr/camera/aruco.h"
#include "kipr/camera/aruco.hpp"

using namespace kipr;
using namespace kipr::camera;

namespace
{
  std::vector<double> pose;
  std::vector<int> ids;
}

void set_aruco_marker_size_cm(double cm) {
  float meters = cm * 0.1000000f;
  Aruco::getInstance()->setArucoMarkerSize(meters);
}

void set_chess_board_square_size_cm(double cm) {
  float meters = cm * 0.1000000f;
  Aruco::getInstance()->setChessBoardSize(meters);
}

signed int set_dictionary(int dictionaryId) {
  // Actually creates the instance
  Aruco::getInstance()->setDictionary(dictionaryId);
}

double *get_marker_pose(int markerId) {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  pose = dev->getPose(markerId);
  return pose.data();
}

int *get_markers_in_view() {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  ids = dev->arucoMarkersInView();
  return ids.data();
}

signed short can_see_marker(int marker) {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  return dev->arucoMarkerInView(marker);
}

