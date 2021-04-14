/*
 * channel_p.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Nafis Zaman
 */

#ifdef WITH_VISION_SUPPORT

#include "channel_p.hpp"
#include "warn.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "wallaby/aruco.hpp"
#include <iostream>
#include <vector>
#include <zbar.h>

using namespace Private::Camera;

HsvChannelImpl::HsvChannelImpl() {}

void HsvChannelImpl::update(const cv::Mat &image) {
  if (image.empty()) {
    m_image = cv::Mat();
    return;
  }

  cv::cvtColor(image, m_image, cv::COLOR_BGR2HSV);
}

Camera::ObjectVector HsvChannelImpl::findObjects(const Config &config) {
  if (m_image.empty())
    return ::Camera::ObjectVector();

  cv::Vec3b top(config.intValue("th"), config.intValue("ts"),
                config.intValue("tv"));
  cv::Vec3b bottom(config.intValue("bh"), config.intValue("bs"),
                   config.intValue("bv"));

  // std::cout << "top: <" << top[0] << ", " << top[1] << ", " << top[2] << ">"
  // << std::endl;

  cv::Mat fixed = m_image;
  if (bottom[0] > top[0]) {
    // Modulo 180
    // TODO: Optimize for ARM?
    const uchar adjH = 180 - bottom[0];
    for (int i = 0; i < fixed.rows; ++i) {
      uchar *row = fixed.ptr<uchar>(i);
      for (int j = 0; j < fixed.cols; ++j) {
        row[j * fixed.elemSize()] += adjH;
        row[j * fixed.elemSize()] %= 180;
      }
    }

    cv::Vec3b adj(adjH, 0, 0);
    bottom = cv::Vec3b(0, bottom[1], bottom[2]);
    cv::add(adj, top, top);
  }

  cv::Mat only;
  cv::inRange(fixed, bottom, top, only);

  std::vector<std::vector<cv::Point>> c;
  cv::findContours(only, c, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);

  std::vector<cv::Moments> m(c.size());
  for (std::vector<cv::Moments>::size_type i = 0; i < c.size(); ++i) {
    m[i] = cv::moments(c[i], false);
  }

  ::Camera::ObjectVector ret;
  for (::Camera::ObjectVector::size_type i = 0; i < c.size(); ++i) {
    const cv::Rect rect = cv::boundingRect(c[i]);
    if (rect.width < 3 && rect.height < 3)
      continue;

    const double confidence = m[i].m00 / rect.area();
    ret.push_back(::Camera::Object(
        Point2<unsigned>(m[i].m10 / m[i].m00, m[i].m01 / m[i].m00),
        Rect<unsigned>(rect.x, rect.y, rect.width, rect.height), confidence));
  }

  return ret;
}

BarcodeChannelImpl::BarcodeChannelImpl() {
  m_image.set_format("Y800");
  m_scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 0);
  m_scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
}

void BarcodeChannelImpl::update(const cv::Mat &image) {
  if (image.empty()) {
    m_gray = cv::Mat();
    return;
  }
#if 1
  cv::cvtColor(image, m_gray, cv::COLOR_BGR2GRAY);
#else
  cv::cvtColor(image, m_gray, CV_BGR2GRAY);
#endif
  m_image.set_data(m_gray.data, m_gray.cols * m_gray.rows);
  m_image.set_size(m_gray.cols, m_gray.rows);
}

::Camera::ObjectVector BarcodeChannelImpl::findObjects(const Config &config) {
  if (m_gray.empty())
    return ::Camera::ObjectVector();

  m_scanner.scan(m_image);
  zbar::SymbolSet symbols = m_scanner.get_results();
  ::Camera::ObjectVector ret;
  zbar::SymbolIterator it = symbols.symbol_begin();
  for (; it != symbols.symbol_end(); ++it) {
    zbar::Symbol symbol = *it;

    // Determine bounding box and centroid
    int left = m_image.get_width();
    int right = 0;
    int bottom = m_image.get_height();
    int top = 0;

    zbar::Symbol::PointIterator pit = symbol.point_begin();
    for (int i = 0; i < symbol.get_location_size(); ++i) {
      const int &x = symbol.get_location_x(i);
      if (x > right)
        right = x;
      if (x < left)
        left = x;

      const int &y = symbol.get_location_y(i);
      if (y > top)
        top = y;
      if (y < bottom)
        bottom = y;
    }

    ret.push_back(::Camera::Object(
        Point2<unsigned>((left + right) / 2, (top + bottom) / 2),
        Rect<unsigned>(left, bottom, right - left, top - bottom), 1.0,
        zbar_symbol_get_data(symbol), zbar_symbol_get_data_length(symbol)));
  }

  return ret;
}

ArucoChannelImpl::ArucoChannelImpl() {}

void ArucoChannelImpl::update(const cv::Mat &image) {
  if (image.empty()) {
    m_image = cv::Mat();
    return;
  }

  // TODO: actual aruco image format needs

  m_image = image;
}

Camera::ObjectVector ArucoChannelImpl::findObjects(const Config &config) {
  if (m_image.empty())
    return ::Camera::ObjectVector();

  // TODO: use m_image, don't get a new image
  // std::cout << "We see " <<
  // aruco::Aruco::getInstance()->arucoMarkersInView(&m_image).size() << "
  // markers" << std::endl;
  ::Camera::ObjectVector ret;
#ifdef ARUCO
  std::vector<std::vector<cv::Point2f>> corners =
      aruco::Aruco::getInstance()->arucoMarkerCorners(&m_image);

  for (auto it = corners.begin(); it != corners.end(); ++it) {
    std::vector<cv::Point2f> marker_corners = *it;

    // cv::Point2f ul = (*it)[0];
    // cv::Point2f ur = (*it)[1];
    // cv::Point2f lr = (*it)[2];
    // cv::Point2f ll = (*it)[3];

    // Determine bounding box and centroid
    int left = m_image.size().height;
    int right = 0;
    int bottom = m_image.size().width;
    int top = 0;

    for (auto it2 = marker_corners.begin(); it2 != marker_corners.end();
         ++it2) {
      if (it2->x < left)
        left = it2->x;
      if (it2->x > right)
        right = it2->x;
      if (it2->y > top)
        top = it2->y;
      if (it2->y < bottom)
        bottom = it2->y;
    }

    ret.push_back(::Camera::Object(
        Point2<unsigned>((left + right) / 2, (top + bottom) / 2),
        Rect<unsigned>(left, bottom, right - left, top - bottom), 1.0));
  }
#endif

  return ret;
}

#endif
