#include "kipr/aruco/aruco.h"
#include "kipr/aruco/aruco.hpp"

using namespace kipr;
using namespace kipr::aruco;

void set_aruco_marker_size_cm(double cm) {
  float meters = cm * 0.1000000f;
  Aruco *dev = Aruco::getInstance();
  dev->setArucoMarkerSize(meters);
}

void set_chess_board_square_size_cm(double cm) {
  float meters = cm * 0.1000000f;
  Aruco *dev = Aruco::getInstance();
  dev->setChessBoardSize(meters);
}

signed int set_dictionary(int dictionaryId) {
  // Actually creates the instance
  Aruco *dev = Aruco::getInstance();
  return dev->setDictionary(dictionaryId);
}

double *get_marker_pose(int markerId) {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  std::vector<double> pose = dev->getPose(markerId);
  return pose.data();
}

int *get_markers_in_view() {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  std::vector<int> ids = dev->arucoMarkersInView();
  return ids.data();
}

signed short can_see_marker(int marker) {
  // TODO proper interface...
  Aruco *dev = Aruco::getInstance();
  return dev->arucoMarkerInView(marker);
}

