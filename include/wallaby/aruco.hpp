/*
 * arhco.h
 *
 * Created on April 4, 2017
 *      Author: Ryan Owens
 */
#ifndef INCLUDE_WALLABY_ARUCO_HPP_
#define INCLUDE_WALLABY_ARUCO_HPP_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <unistd.h>

#include "wallaby/camera.hpp"

class Aruco {
public:
  Aruco(int dicionaryId);
  std::vector<double> getPose(int arucoId);
  std::vector<int> arucoMarkersInView();
  bool arucoMarkerInView(int arucoId);
  bool setCameraCalibration(std::string filename);
  bool calibrate();

private:
  float chessBoardSquareSize = 0.0235f; // Meters // TODO pass in this value?
  float arucoSquareSize = 0.025f;       // Meters // TODO pass in this value?
  int numImagesForCalibration = 15;     // TODO appropriate value?
  cv::Size chessBoardDimensions = cv::Size(6, 9);
  std::vector<cv::Mat> images;
  std::vector<std::vector<cv::Point2f>> foundChessboardCorners;
  cv::Mat cameraMatrix, distortionCoefficients;
  int dictionaryId;
  int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH |
                        cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK;
  std::string calibrationFile =
      "calibration.txt";                 // TODO need to get the proper path
  std::string customDictionaryFile = ""; // TODO need to get the proper path
  std::string newCalibrationFile = "";   // TODO neet to get the proper path
  cv::Ptr<cv::aruco::Dictionary> dictionary;
  cv::Ptr<cv::aruco::DetectorParameters> detectorParams;

  cv::Mat getFrame();
  bool saveCalibration();
  void getImagesFromCamera();
  bool getCustomDictionary();
  bool readCameraCalibration(std::string filename);
  void calculateChessBoardPosition(std::vector<cv::Point3f> &vec);
  bool vectorContains(std::vector<int> vec, int val);
  void calculateChessBoardCornersFromImages(
      std::vector<std::vector<cv::Point2f>> &foundCorners);
};
#endif
