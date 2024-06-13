#include "utils_parse.h"

#include <bits/stdc++.h>

double UtilsParse::ParseDouble(const uint8_t* _buffer) {
  double x;
  std::copy(_buffer, _buffer + sizeof(double), reinterpret_cast<uint8_t*>(&x));
  return x;
}

bool UtilsParse::ParseDouble(const std::string& string, double& value) {
  if (string.empty()) {
    return false;
  }

  char* end;
  errno = 0;
  double number = strtod(string.c_str(), &end);

  // Check if an error occured or if there are junk characters at the end.
  if (errno != 0 || end != string.c_str() + string.length()) {
    return false;
  }

  value = number;
  return true;
}

float UtilsParse::ParseFloat(const uint8_t* _buffer) {
  float x;
  std::copy(_buffer, _buffer + sizeof(float), reinterpret_cast<uint8_t*>(&x));
  return x;
}

bool UtilsParse::ParseFloat(const std::string& string, float& value) {
  if (string.empty()) {
    return false;
  }

  char* end;
  errno = 0;
  float number = strtof(string.c_str(), &end);

  // Check if an error occured or if there are junk characters at the end.
  if (errno != 0 || end != string.c_str() + string.length()) {
    return false;
  }

  value = number;
  return true;
}

int16_t UtilsParse::ParseInt16(const uint8_t* buffer) {
  int16_t number;
  std::copy(buffer, buffer + 2, reinterpret_cast<uint8_t*>(&number));
  return number;
}

int32_t UtilsParse::ParseInt32(const uint8_t* _buffer) {
  int32_t number;
  std::copy(_buffer, _buffer + 4, reinterpret_cast<uint8_t*>(&number));
  return number;
}

bool UtilsParse::ParseInt32(const std::string& string, int32_t& value,
                            int32_t base) {
  if (string.empty()) {
    return false;
  }

  char* end;
  errno = 0;
  int64_t number = strtol(string.c_str(), &end, base);

  // Check if an error occured or if there are junk characters at the end.
  if (errno != 0 || end != string.c_str() + string.length()) {
    return false;
  }

  if (number > std::numeric_limits<int32_t>::max() ||
      number < std::numeric_limits<int32_t>::min()) {
    return false;
  }

  value = number;
  return true;
}

uint32_t UtilsParse::ParseUInt32(const uint8_t* _buffer) {
  uint32_t number;
  std::copy(_buffer, _buffer + 4, reinterpret_cast<uint8_t*>(&number));
  return number;
}

bool UtilsParse::ParseUInt32(const std::string& string, uint32_t& value,
                             int32_t base) {
  if (string.empty()) {
    return false;
  }

  char* end;
  errno = 0;
  int64_t number = strtol(string.c_str(), &end, base);

  // Check if an error occured or if there are junk characters at the end.
  if (errno != 0 || end != string.c_str() + string.length()) {
    return false;
  }

  if (number > std::numeric_limits<uint32_t>::max() || number < 0) {
    return false;
  }

  value = number;
  return true;
}

double UtilsParse::ConvertDmsToDegrees(double dms) {
  uint32_t whole_degrees = static_cast<uint32_t>(dms) / 100;
  double minutes = dms - static_cast<double>(whole_degrees * 100);
  // 60 minutes in a degree
  double degrees = static_cast<double>(whole_degrees) + minutes / 60.0;
  return degrees;
}
