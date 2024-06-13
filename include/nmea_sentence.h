#pragma once

#include <string>
#include <vector>

/**
 * @brief NMEA消息类型
 */
enum class NmeaType {
  NMEA_NULL = 0,  // 无NMEA有效信息
  NMEA_GPGGA,     // NMEA 定位信息
  NMEA_GPGSA,     // NMEA 卫星信息
  NMEA_GPGST,     // NMEA 伪距观测噪声
  NMEA_GPGSV,     // NMEA 可视卫星状态
  NMEA_GPHDT,     // NMEA 航向信息
  NMEA_GPRMC,     // NMEA 推荐定位信息(时间+经纬度)
  NMEA_GPZDA,      // NMEA 时间信息
  NMEA_GPGLL,     // NMEA 无方差定位信息
};

/**
 * @brief GPGGA Info
 *
 */
struct GPGGAInfo {
  double utc_time;

  double latitude;

  double longitude;

  double altitude;

  double hdop;

  int gps_status;

  int num_satellites;
};

/**
 * @brief GPGST Info
 *
 */
struct GPGSTInfo {
  double utc_time;

  double latitude_std;

  double longitude_std;

  double altitude_std;
};

struct GPGLLInfo{
  double utc_time;

  double latitude;

  double longitude;
};

/**
 * @brief NMEA解析语句
 */
struct NmeaSentence {
  NmeaType type;
  std::vector<std::string> body;
};