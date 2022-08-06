/*
 * aruco.h
 *
 *  Created on: April 10, 2017
 *      Author: Ryan Owens
 */

#ifdef ARUCO

#ifndef INCLUDE_WALLABY_ARUCO_H_
#define INCLUDE_WALLABY_ARUCO_H_

/*!
 * \file aruco.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup aruco ArucoMarkers
 */
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Sets the dictionary to use for Aruco Markers
 * \description dictionary id where < 0 is custom dictionary
 * \description Gets and sets the Dictionary to use for Aruco Markers
 * \return signed int
 * \ingroup aruco
 */
signed int set_dictionary(int dicionaryId);

/*!
 * \brief Sets the Arcuo Marker Size
 * \description set the aruco marker size in cm
 * \return void
 * \ingroup aruco
 */
void set_aruco_marker_size_cm(double cm);

/*!
 * \brief Sets the Chess Board Square Size
 * \description set the calibration chess board square size in cm
 * \return void
 * \ingroup aruco
 */
void set_chess_board_square_size_cm(double cm);

/*!
 * \brief Get XYZ RXRYRZ of an Aruco Marker
 * \description marker of current dictionary
 * \description Gets the Rotation and Translation vector of an Aruco Marker
 * \return double*
 * \ingroup aruco
 */
double *get_marker_pose(int markerId);

/*!
 * \brief Checks if a marker is in view
 * \description marker id of current dictionary
 * \description Checks if an Aruco Marker can be seen in the current view
 * \return signed short
 * \ingroup aruco
 */
signed short can_see_marker(int marker);

/*!
 * \brief Runs camera calibration
 * \description Runs a camera calibration TODO
 * \return signed short
 * \ingroup aruco
 */
// signed short camera_calibrate();

/*!
 * \brief Gets an array of all markers in view
 * \description TODO
 * \description  Gets an array of all Aruco markers in view
 * \return int*
 * \ingroup aruco
 */
int *get_markers_in_view();

#ifdef __cplusplus
}
#endif
#endif
#endif
