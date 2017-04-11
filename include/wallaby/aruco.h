/*
 * aruco.h
 *
 *  Created on: April 10, 2017
 *      Author: Ryan Owens
 */

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
     * \return none
     * \ingroup aruco
     */
     void set_dictionary(int dicionaryId);

     /*!
      * \brief Get XYZ RXRYRZ of an Aruco Marker
      * \description marker of current dictionary
      * \description Gets the Rotation and Translation vector of an Aruco Marker
      * \return double*
      * \ingroup aruco
      */
      double* get_marker_pose(int markerId);

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
         int* get_markers_in_view();

    #ifdef __cplusplus
    }
    #endif
 #endif
