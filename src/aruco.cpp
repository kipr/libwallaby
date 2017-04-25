/*
 * battery.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: Ryan Owens
 */

#include "wallaby/aruco.hpp"
#include "camera_c_p.hpp"

#ifdef ARUCO
/*
 * Aruco
 *
 * Class for detecting Aruco markers and getting Pose Estimation
 *
 */
aruco::Aruco::Aruco(int dictionaryId) {
  this->dictionaryId = dictionaryId;
  // if dictioniary ID < 0 use custom dictionary file
  this->dictionaryId = dictionaryId;
  if (dictionaryId < 0) {
    this->getCustomDictionary();
  } else {
    this->dictionary = cv::aruco::getPredefinedDictionary(
        cv::aruco::PREDEFINED_DICTIONARY_NAME(this->dictionaryId));
  }
  this->detectorParams = cv::aruco::DetectorParameters::create();
  this->detectorParams->doCornerRefinement = true;
  // sets WHITE_2016 custom configuration as default
  std::string fname = this->calibrationFilePath + this->WHITE_CAMERA_FILE;
  if (access(fname.c_str(), F_OK) != -1) {
    this->currentCalibrationFile = this->WHITE_CAMERA_FILE;
    // Now get distortion calibration data
    this->readCameraCalibration(this->currentCalibrationFile);
  }
  // this->m_camDevice = new Camera::Device();
  std::cout << "Aruco constructor" << std::endl;
  // TODO: ??? this->m_camDevice->open(0, LOW_RES, BLACK_2017);
}

/*
 * get Instance
 *
 * Get an instance of the Aruco class
 *
 */
aruco::Aruco *aruco::Aruco::getInstance() {
  if (!aruco::instance)
    aruco::instance = new Aruco(aruco::defaultDictionaryID);
  return aruco::instance;
}

/*
 * Aruco
 *
 * Class Destructor
 *
 */
aruco::Aruco::~Aruco() {
  // need to close?
  // bool ret = Private::DeviceSingleton::instance()->open();
}

/*
 * Aruco
 *
 * Class for detecting Aruco markers and getting Pose Estimation
 *
 */
cv::Mat aruco::Aruco::getFrame() {
  cv::Mat mt;
  // if (!this->m_camDevice->update())
  //   return mt;
  // return m_camDevice->rawImage();
  if (!Private::DeviceSingleton::instance()->update())
    return mt;
  return Private::DeviceSingleton::instance()->rawImage();
}
/*
 * Set Camera Calibration
 *
 * Updates the Camera Distortion Matrixes from the file
 *
 */
bool aruco::Aruco::setCameraCalibration(std::string filename) {
  std::string fname = this->calibrationFilePath + filename;
  if (access(fname.c_str(), F_OK) == -1)
    return false;
  this->currentCalibrationFile = filename;
  return this->readCameraCalibration(filename);
}

/*
 * Read Camera Calibration
 *
 * Reads in the Camera Callibration for use with Pose Estimation
 *
 */
bool aruco::Aruco::readCameraCalibration(std::string filename) {
  cv::FileStorage fs(this->calibrationFilePath + filename,
                     cv::FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["camera_matrix"] >> this->cameraMatrix;
  fs["distortion_coefficients"] >> this->distortionCoefficients;
  fs.release();
  return true;
}

/*
 * Get Custom Dictionary
 *
 * Sets the Dictionary to a custom dictionary
 *
 */
bool aruco::Aruco::setDictionary(int dictionaryId) {
  if (dictionaryId < 0) {
    this->dictionaryId = dictionaryId;
    return getCustomDictionary();
  } else {
    this->dictionaryId = dictionaryId;
    this->dictionary = cv::aruco::getPredefinedDictionary(
        cv::aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));
    return true;
  }
}
/*
 * Get Custom Dictionary
 *
 * Sets the Dictionary to a custom dictionary
 *
 */
bool aruco::Aruco::getCustomDictionary() {
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

/*
 * Vector Contains
 *
 * Helper function to check if a particular integer Vector Contains
 * a value
 *
 */
bool aruco::Aruco::vectorContains(std::vector<int> vec, int val) {
  if (find(vec.begin(), vec.end(), val) != vec.end())
    return true;
  return false;
}

/*
 * Get Pose
 *
 * Get the X, Y, and Z Rotation and Translation vectors for a
 * particular Aruco Marker in the View
 *
 * returns all zeros when not in View
 * returns as TX TY TZ RX RY RZ
 *
 */
std::vector<double> aruco::Aruco::getPose(int arucoId, cv::Mat *frame) {
  std::vector<double> rottransvec;
  rottransvec.assign(6, 0.0);
  // Camera Calibration Failed...No files?
  if ((cameraMatrix.empty() || cv::countNonZero(cameraMatrix) < 1) ||
      (distortionCoefficients.empty() ||
       cv::countNonZero(distortionCoefficients) < 1))
    return rottransvec;

  cv::Mat img;
  if (frame == nullptr) {
    if (!Private::DeviceSingleton::instance()->isOpen())
      if (!this->openCamera())
        return rottransvec;
    img = this->getFrame();
  } else {
    img = *frame;
  }

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

/*
 * Markers in View
 *
 * Get an array (vector) of all Aruco Markers in the current view
 *
 */
std::vector<int> aruco::Aruco::arucoMarkersInView(cv::Mat *frame) {
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  cv::Mat img;
  if (frame == nullptr) {
    // if (!this->m_camDevice->isOpen())
    if (!Private::DeviceSingleton::instance()->isOpen())
      if (!this->openCamera())
        return ids;
    img = this->getFrame();
  } else {
    img = *frame;
  }

  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);

  return ids;
}

/*
 * Marker corners in view
 *
 */
std::vector<std::vector<cv::Point2f>>
aruco::Aruco::arucoMarkerCorners(cv::Mat *frame) {
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;

  cv::Mat img;
  if (frame == nullptr) {
    // if (!this->m_camDevice->isOpen())
    if (!Private::DeviceSingleton::instance()->isOpen())
      if (!this->openCamera())
        return corners;
    img = this->getFrame();
  } else {
    img = *frame;
  }

  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);

  return corners;
}

/*
 * Marker In View
 *
 * Check if a particular Aruco Marker is in the current view
 *
 */
bool aruco::Aruco::arucoMarkerInView(int arucoId, cv::Mat *frame) {
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  cv::Mat img;
  if (frame == nullptr) {
    // if (!this->m_camDevice->isOpen())
    if (!Private::DeviceSingleton::instance()->isOpen())
      if (!this->openCamera())
        return false;
    img = this->getFrame();
  } else {
    img = *frame;
  }
  cv::aruco::detectMarkers(img, this->dictionary, corners, ids, detectorParams,
                           rejected);
  if (find(ids.begin(), ids.end(), arucoId) != ids.end())
    return true;
  return false;
}

/*
 * Calculate Chess Board Position
 *
 * Determines the chess board Position
 *
 */
void aruco::Aruco::calculateChessBoardPosition(
    std::vector<cv::Point3f> &corners) {
  for (size_t i = 0; i < (size_t)this->chessBoardDimensions.height; i++) {
    for (size_t j = 0; j < (size_t)this->chessBoardDimensions.width; j++) {
      // X = j * square size, Y = i * square size, Z = 0.0
      corners.push_back(cv::Point3f(j * this->chessBoardSquareSize,
                                    i * this->chessBoardSquareSize, 0.0f));
    }
  }
}

/*
 * Calculate Chess Board Corners
 *
 * Determines all the chess board corner positions
 *
 */
void aruco::Aruco::calculateChessBoardCornersFromImages(
    std::vector<std::vector<cv::Point2f>> &foundCorners) {
  for (std::vector<cv::Mat>::iterator iter = this->images.begin();
       iter != this->images.end(); iter++) {
    std::vector<cv::Point2f> pointBuf;
    bool chessBoardFound = findChessboardCorners(
        *iter, this->chessBoardDimensions, pointBuf, this->chessBoardFlags);
    if (chessBoardFound) {
      foundCorners.push_back(pointBuf);
    }
  }
}

/*
 * Get Images From Camera
 *
 * Grab images from a camera and store for calibration
 *
 */

void aruco::Aruco::getImagesFromCamera() {
  cv::Mat frame, drawToFrame, oefficients,
      cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  std::vector<cv::Mat> savedImages;
  std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCorners;

  // if (!this->m_camDevice->isOpen())
  if (!Private::DeviceSingleton::instance()->isOpen())
    if (!this->openCamera())
      return;
  usleep(5 * 1000000);
  while (true) {
    // TODO Setup using LibWallaby Camera
    // if (!this->inputVideo.read(frame))
    //   break;

    std::vector<cv::Vec2f> foundPoints;
    bool found = false;
    found = findChessboardCorners(frame, this->chessBoardDimensions,
                                  foundPoints, this->chessBoardFlags);

    if (this->images.size() >= this->numImagesForCalibration)
      return;
    if (found) {
      cv::Mat temp;
      frame.copyTo(temp);
      this->images.push_back(temp);
      // TODO Display to User to positon for next Frame...
      usleep(5 * 1000);
    } else {
      // TODO Display to User to reposition camera...
    }
  }
}

/*
 * Get Images From Camera
 *
 * Grab images from a camera and store for calibration
 *
 */
bool aruco::Aruco::calibrate(std::vector<cv::Mat> images) {
  this->images = images; // pass in images instead of get them here
  std::vector<std::vector<cv::Point2f>> imageSpacePoints;
  this->calculateChessBoardCornersFromImages(imageSpacePoints);
  std::vector<std::vector<cv::Point3f>> worldCornerPoints(1);
  this->calculateChessBoardPosition(worldCornerPoints[0]);
  worldCornerPoints.resize(imageSpacePoints.size(), worldCornerPoints[0]);

  std::vector<cv::Mat> rVecs, tVecs;
  this->distortionCoefficients = cv::Mat::zeros(8, 1, CV_64F);
  // Performs the Calibration
  calibrateCamera(worldCornerPoints, imageSpacePoints,
                  this->chessBoardDimensions, this->cameraMatrix,
                  this->distortionCoefficients, rVecs, tVecs);
  if (this->saveCalibration())
    return true;
  else
    return false;
}

/*
 * Save Calibration
 *
 * Save the custom calibration file
 *
 */
bool aruco::Aruco::saveCalibration() {
  std::string fname = "";
  // if (this->m_camDevice->getModel() == WHITE_2016) {
  if (Private::DeviceSingleton::instance()->getModel() == WHITE_2016) {
    fname = this->calibrationFilePath + this->WHITE_CAMERA_FILE;
  } else {
    fname = this->calibrationFilePath + this->BLACK_CAMERA_FILE;
  }
  cv::FileStorage fs(fname, cv::FileStorage::WRITE);
  if (!fs.isOpened())
    return false;
  time_t tm;
  time(&tm);
  struct tm *t2 = localtime(&tm);
  char buf[1024];
  strftime(buf, sizeof(buf), "%c", t2);

  fs << "calibration_time" << buf;
  fs << "camera_matrix" << this->cameraMatrix;
  fs << "distortion_coefficients" << this->distortionCoefficients;
  fs.release();
  this->setCameraCalibration(this->calibrationFilePath);
  return true;
}

/*
 * Set Chess Board Size
 *
 * Sets the size for the chess board squares
 *
 */
void aruco::Aruco::setChessBoardSize(float sizeInMeters) {
  if (sizeInMeters > 0.0 && sizeInMeters < 1.0) {
    this->chessBoardSquareSize = sizeInMeters;
  } else {
    this->chessBoardSquareSize = 0.0235f; // Default size if printed on 8.5x11
  }
}

/*
 * Set Aruco Marker Size
 *
 * Sets the size for aruco markers
 *
 */
void aruco::Aruco::setArucoMarkerSize(float sizeInMeters) {
  if (sizeInMeters > 0.0 && sizeInMeters < 1.0) {
    this->arucoSquareSize = sizeInMeters;
  } else {
    this->arucoSquareSize = 0.025f; // Default size if printed on 8.5x11
  }
}

/*
 * Open Camera
 *
 * Opens the Camera
 *
 */
bool aruco::Aruco::openCamera() {
  // TODO allow multiple cameras
  if (Private::DeviceSingleton::instance()->isOpen())
    return true;
  // return this->m_camDevice->open() && this->m_camDevice->isOpen();
  return Private::DeviceSingleton::instance()->open();
}

#endif
