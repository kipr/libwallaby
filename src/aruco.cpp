/*
 * battery.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: Ryan Owens
 */

#include "wallaby/aruco.hpp"

Aruco::Aruco(int dictionaryId) {
  this->dictionaryId = dictionaryId;
  // if dictioniary ID < 0 use custom dictionary file
  if (dictionaryId < 0)
    this->getCustomDictionary();
  else
    this->dictionary = cv::aruco::getPredefinedDictionary(
        cv::aruco::PREDEFINED_DICTIONARY_NAME(this->dictionaryId));
  this->detectorParams = cv::aruco::DetectorParameters::create();
  this->detectorParams->doCornerRefinement = true;
  this->readCameraCalibration(this->calibrationFile);
}

bool Aruco::setCameraCalibration(std::string filename) {
  return this->readCameraCalibration(filename);
}

bool Aruco::readCameraCalibration(std::string filename) {
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["camera_matrix"] >> this->cameraMatrix;
  fs["distortion_coefficients"] >> this->distortionCoefficients;
  fs.release();
  return true;
}

bool Aruco::getCustomDictionary() {
  int markerSize, maxCorrectionBits;
  cv::Mat bytesList;
  cv::FileStorage fs(this->customDictionaryFile, cv::FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["MarkerSize"] >> markerSize;
  fs["MaxCorrectionBits"] >> maxCorrectionBits;
  fs["BytesList"] >> bytesList;
  this->dictionary =
      new cv::aruco::Dictionary(bytesList, markerSize, maxCorrectionBits);
  return true;
}

bool Aruco::vectorContains(std::vector<int> vec, int val) {
  if (find(vec.begin(), vec.end(), val) != vec.end())
    return true;
  return false;
}

std::vector<double> Aruco::getPose(int arucoId) {
  std::vector<double> rottransvec;
  rottransvec.assign(6, 0.0);
  // TODO setup using Libwallaby Camera
  // if(!this->isCameraOpen()) return rottransvec;

  // TODO setup using Libwallaby Camera
  // cv::Mat img = this->getFrame();
  cv::Mat img;
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  std::vector<cv::Vec3d> rvecs, tvecs;

  // detect markers and estimate pose
  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);
  if (ids.size() > 0 && this->vectorContains(ids, arucoId)) {
    cv::aruco::estimatePoseSingleMarkers(corners, this->arucoSquareSize,
                                         cameraMatrix, distortionCoefficients,
                                         rvecs, tvecs);

    size_t index = find(ids.begin(), ids.end(), arucoId) - ids.begin();
    cv::Vec3d translation = tvecs[index];
    cv::Vec3d rotation = rvecs[index];
    for (size_t i = 0; i < translation.rows; i++) {
      rottransvec.push_back(translation[i]);
    }

    for (size_t i = 0; i < rotation.rows; i++) {
      rottransvec.push_back(rotation[i]);
    }
  }
  return rottransvec;
}

std::vector<int> Aruco::arucoMarkersInView() {
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  // TODO Setup using Libwallaby Camera
  // if(!this->isCameraOpen()) return ids;
  // TODO Setup using Libwallaby Camera
  // Mat img = this->getFrame();
  cv::Mat img;
  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);
  return ids;
}

bool Aruco::arucoMarkerInView(int arucoId) {
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  // TODO Setup using Libwallaby Camera
  // if(!this->isCameraOpen()) return false;
  // TODO Setup using Libwallaby Camera
  // Mat img = this->getFrame();
  cv::Mat img;
  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);
  if (find(ids.begin(), ids.end(), arucoId) != ids.end())
    return true;
  return false;
}
