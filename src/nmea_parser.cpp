#include "nmea_parser.h"

#include <limits.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iomanip>

#include "utils_parse.h"
// #include "utils/utils_log.h"

const std::string NmeaParser::CHECKSUM_FLAG = "*";
const std::string NmeaParser::FIELD_SEPARATOR = ",";
const std::string NmeaParser::HEADER_SEPARATOR = ";";
const std::string NmeaParser::NMEA_SENTENCE_FLAG = "$";
const std::string NmeaParser::NMEA_ENDLINE = "\r\n";

NmeaParser::NmeaParser() {
  nmea_type_string_map_ = {
      {"GPGGA", NmeaType::NMEA_GPGGA}, {"GPGSA", NmeaType::NMEA_GPGSA},
      {"GPGST", NmeaType::NMEA_GPGST}, {"GPGSV", NmeaType::NMEA_GPGSV},
      {"GPHDT", NmeaType::NMEA_GPHDT}, {"GPRMC", NmeaType::NMEA_GPRMC},
      {"GPZDA", NmeaType::NMEA_GPZDA}, {"GNGGA", NmeaType::NMEA_GPGGA},
      {"GNGSA", NmeaType::NMEA_GPGSA}, {"GNGST", NmeaType::NMEA_GPGST},
      {"GNGSV", NmeaType::NMEA_GPGSV}, {"GNHDT", NmeaType::NMEA_GPHDT},
      {"GNRMC", NmeaType::NMEA_GPRMC}, {"GNZDA", NmeaType::NMEA_GPZDA},
      {"GPGLL", NmeaType::NMEA_GPGLL}};
}

NmeaType NmeaParser::ParseNmeaMessage(const std::string& _message,
                                      double _message_timestamp) {
  // rtk_gps_cache_.clear();
  NmeaType result_type = NmeaType::NMEA_NULL;
  std::string remaining_message;
  std::vector<NmeaSentence> nmea_sentences;
  _nmea_message.insert(_nmea_message.end(), _message.begin(), _message.end());
  if (!ExtractCompleteMessages(_nmea_message, nmea_sentences,
                               remaining_message)) {
    return result_type;
  }
  _nmea_message = remaining_message;
  bool gpgga_flag = false;
  bool gpgst_flag = false;
  bool gpgll_flag = false;

  for (size_t i = 0; i < nmea_sentences.size(); i++) {
    // std::cout << "nmea type : " << (int)nmea_sentences[i].type << std::endl;
    // for (size_t j = 0; j < nmea_sentences[i].body.size(); j++) {
    //     std::cout << nmea_sentences[i].body[j] << " ";
    // }
    // std::cout << std::endl;

    switch (nmea_sentences[i].type) {
      case NmeaType::NMEA_GPGGA:
        ParseGPGGA(nmea_sentences[i], gpgga_info);
        gpgga_flag = true;
        result_type = NmeaType::NMEA_GPGGA;
        break;
      case NmeaType::NMEA_GPGSA:
        // TODO code for parsing GPGSA Data
        break;
      case NmeaType::NMEA_GPGST:
        ParseGPGST(nmea_sentences[i], gpgst_info);
        gpgst_flag = true;
        result_type = NmeaType::NMEA_GPGST;
        break;
      case NmeaType::NMEA_GPGSV:
        // TODO code for parsing GPGSV  Data
        break;
      case NmeaType::NMEA_GPHDT:
        // TODO code for parsing GPHDT  Data
        break;
      case NmeaType::NMEA_GPRMC:
        // TODO code for parsing GPRMC  Data
        break;
      case NmeaType::NMEA_GPZDA:
        // TODO code for parsing GPZDA  Data
        break;
      case NmeaType::NMEA_GPGLL:
        ParseGPGLL(nmea_sentences[i], gpgll_info);
        gpgll_flag = true;
        result_type = NmeaType::NMEA_GPGLL;
        break;
        break;

      default:
        break;
    }
    if (!gpgga_flag && !gpgst_flag && !gpgll_flag) {
      return result_type;
    }

    // SmileSMower::Header* header(new SmileSMower::Header);
    // header->set_timestamp_sec(_message_timestamp);
    // rtk_msg.set_allocated_header(header);

    if (gpgga_flag) {
      // SmileSMower::PointLLH* llh_msg(new SmileSMower::PointLLH);
      // llh_msg->set_lat(gpgga_info.latitude);
      // llh_msg->set_lon(gpgga_info.longitude);
      // llh_msg->set_height(gpgga_info.altitude);

      // rtk_msg.set_allocated_llh(llh_msg);
      // rtk_msg.set_status(gpgga_info.gps_status);
      // rtk_msg.set_num_satellite(gpgga_info.num_satellites);
    }

    if (gpgst_flag) {
      // SmileSMower::Point3D* cov_std_msg(new SmileSMower::Point3D);
      // cov_std_msg->set_x(gpgst_info.latitude_std);
      // cov_std_msg->set_y(gpgst_info.longitude_std);
      // cov_std_msg->set_z(gpgst_info.altitude_std);
      // rtk_msg.set_allocated_cov_std(cov_std_msg);
    }

    if (gpgga_info.utc_time == gpgst_info.utc_time) {
      // rtk_gps_cache_.push_back(rtk_msg);
    }
  }
  return result_type;
}

// std::vector<SmileSMower::RtkGPS> NmeaParser::GetLatestRtkData() {
//     std::vector<SmileSMower::RtkGPS> temp = rtk_gps_cache_;
//     rtk_gps_cache_.clear();
//     return temp;
// }

void NmeaParser::FindAsciiSentence(const std::string& _sentense,
                                   size_t _current_idx, size_t& _start_idx,
                                   size_t& _end_idx) {
  _start_idx = _sentense.find_first_of(NMEA_SENTENCE_FLAG, _current_idx);
  _end_idx = std::string::npos;
  if (_start_idx == std::string::npos) {
    return;
  }

  _end_idx = _sentense.find(NMEA_ENDLINE, _start_idx);
}

size_t NmeaParser::GetCheckSumStart(const std::string& _sentense,
                                    size_t _start_idx) {
  return _sentense.find(CHECKSUM_FLAG, _start_idx);
}

uint8_t NmeaParser::NmeaCheckSum(const std::string& _sentence) {
  uint8_t checksum = 0;
  std::string::const_iterator it = _sentence.begin();
  for (; it != _sentence.end(); ++it) {
    checksum ^= *it;
  }
  return checksum;
}

int32_t NmeaParser::GetNmeaSentence(const std::string& _str, size_t _start_idx,
                                    std::string& _sentence) {
  _sentence.clear();

  size_t checksum_start = GetCheckSumStart(_str, _start_idx);
  if (checksum_start == std::string::npos) {
    // Sentence not complete. Just return.
    return -1;
  } else if (checksum_start + 2 >= _str.size()) {
    // Sentence not complete. Just return.
    return -1;
  } else {
    // Compare the checksums
    _sentence = _str.substr(_start_idx + 1, checksum_start - _start_idx - 1);
    std::string checksum_str = _str.substr(checksum_start + 1, 2);
    // checksum in str
    uint64_t checksum = std::strtoul(checksum_str.c_str(), nullptr, 16);
    uint64_t calculated_checksum = NmeaCheckSum(_sentence);
    // std::cout << "check sum " << checksum << " : " << calculated_checksum <<
    // std::endl;

    if (checksum == ULONG_MAX) {
      // Invalid checksum
      return 0;
    } else if (static_cast<uint32_t>(checksum) == calculated_checksum) {
      return 1;
    } else {
      return 0;
    }
  }
}

void NmeaParser::VectorizeNmeaSentence(const std::string& _sentence,
                                       NmeaSentence& _vectorized_message) {
  boost::algorithm::split(_vectorized_message.body, _sentence,
                          boost::algorithm::is_any_of(FIELD_SEPARATOR));
  if (!_vectorized_message.body.empty()) {
    auto iter = nmea_type_string_map_.find(_vectorized_message.body.front());
    if (iter != nmea_type_string_map_.end()) {
      _vectorized_message.type = iter->second;
    } else {
      _vectorized_message.type = NmeaType::NMEA_NULL;
    }
  }
}

bool NmeaParser::ExtractCompleteMessages(
    std::string _message, std::vector<NmeaSentence>& _nmea_senstence,
    std::string& _remaining) {
  bool parse_error = false;

  size_t sentence_start = 0;
  while (sentence_start != std::string::npos &&
         sentence_start < _message.size()) {
    size_t ascii_start_idx;
    size_t ascii_end_idx;
    FindAsciiSentence(_message, sentence_start, ascii_start_idx, ascii_end_idx);

    if (ascii_start_idx == std::string::npos) {
      break;
    }

    size_t ascii_len = ascii_end_idx - ascii_start_idx;
    std::string cur_sentence;
    int32_t result = GetNmeaSentence(_message, ascii_start_idx, cur_sentence);
    // Checksum verification success
    if (result > 0) {
      _nmea_senstence.emplace_back(NmeaSentence());
      VectorizeNmeaSentence(cur_sentence, _nmea_senstence.back());
      sentence_start = ascii_end_idx;
    } else if (result < 0) {
      // TODO 是否将不完整str的保存
      _remaining.insert(_remaining.end(), _message.begin() + ascii_start_idx,
                        _message.end());
      break;

    } else {
      printf("Invalid NMEA checksum for: [%s]",
             _message.substr(ascii_start_idx, ascii_len).c_str());
      sentence_start += 1;
      parse_error = true;
    }
  }

  return !parse_error;
}

bool NmeaParser::ParseGPGGA(const NmeaSentence& _sentence,
                            GPGGAInfo& _gpgga_info) {
  const size_t MAX_LEN = 15;
  const size_t MIN_LEN = 14;
  if (_sentence.body.size() > MAX_LEN || _sentence.body.size() < MIN_LEN) {
    std::stringstream error;
    error << "Expected GPGGA length " << MIN_LEN << "  <= length <= " << MAX_LEN
          << ", actual length = " << _sentence.body.size();
    // throw ParseException(error.str());
    return false;
  }

  bool valid_flag = true;
  double utc_time = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[1], utc_time);

  double latitude = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[2], latitude);
  latitude = UtilsParse::ConvertDmsToDegrees(latitude);

  double longitude = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[4], longitude);
  longitude = UtilsParse::ConvertDmsToDegrees(longitude);

  uint32_t gps_status, num_satellites;
  valid_flag =
      valid_flag && UtilsParse::ParseUInt32(_sentence.body[6], gps_status);
  valid_flag =
      valid_flag && UtilsParse::ParseUInt32(_sentence.body[7], num_satellites);

  double hdop, altitude;
  valid_flag = valid_flag && UtilsParse::ParseDouble(_sentence.body[8], hdop);
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[9], altitude);

  // std::cout << std::setprecision(8) << latitude << " : " << longitude << " :
  // " << altitude << " : " << gps_status
  //           << " : " << num_satellites << " : " << hdop << std::endl;

  _gpgga_info.utc_time = utc_time;
  _gpgga_info.latitude = latitude;
  _gpgga_info.longitude = longitude;
  _gpgga_info.altitude = altitude;
  _gpgga_info.hdop = hdop;
  _gpgga_info.num_satellites = num_satellites;
  _gpgga_info.gps_status = gps_status;

  std::cout << "latitude: " << std::setprecision(8) << latitude << std::endl;
  std::cout << "longitude: " << std::setprecision(8) << longitude << std::endl;

  return valid_flag;
}

bool NmeaParser::ParseGPGST(const NmeaSentence& _sentence,
                            GPGSTInfo& _gpgst_info) {
  const size_t MESSAGE_LEN = 9;
  if (_sentence.body.size() != MESSAGE_LEN) {
    std::stringstream error;
    error << "Expected GPGST length " << MESSAGE_LEN
          << ", actual length = " << _sentence.body.size();
    // throw ParseException(error.str());
    return false;
  }

  bool valid_flag = true;
  double utc_time = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[1], utc_time);

  double lat_std = 0.0;
  double lon_std = 0.0;
  double alt_std = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[6], lat_std);
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[7], lon_std);
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[8], alt_std);

  _gpgst_info.utc_time = utc_time;
  _gpgst_info.latitude_std = lat_std;
  _gpgst_info.longitude_std = lon_std;
  _gpgst_info.altitude_std = alt_std;

  std::cout << "*** gpgst ***: " << std::endl;
  std::cout << "latitude_std: " << std::setprecision(8) << lat_std << std::endl;
  std::cout << "longitude_std: " << std::setprecision(8) << lon_std
            << std::endl;
  std::cout << "altitude_std: " << std::setprecision(8) << alt_std << std::endl;
  std::cout << "utc_time: " << std::setprecision(8) << utc_time << std::endl;

  return valid_flag;
}

bool NmeaParser::ParseGPGLL(const NmeaSentence& _sentence,
                            GPGLLInfo& _gpgll_info) {
  const size_t MESSAGE_LEN = 8;
  if (_sentence.body.size() != MESSAGE_LEN) {
    std::stringstream error;
    error << "Expected GPGST length " << MESSAGE_LEN
          << ", actual length = " << _sentence.body.size();
    // throw ParseException(error.str());
    return false;
  }

  bool valid_flag = true;

  double latitude = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[1], latitude);
  latitude = UtilsParse::ConvertDmsToDegrees(latitude);

  double longitude = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[3], longitude);
  longitude = UtilsParse::ConvertDmsToDegrees(longitude);

  double utc_time = 0.0;
  valid_flag =
      valid_flag && UtilsParse::ParseDouble(_sentence.body[5], utc_time);

  _gpgll_info.utc_time = utc_time;
  _gpgll_info.longitude = longitude;
  _gpgll_info.latitude = latitude;

  std::cout << "*** gpgll ***: " << std::endl;
  std::cout << "latitude: " << std::setprecision(8) << latitude << std::endl;
  std::cout << "longitude: " << std::setprecision(8) << longitude << std::endl;
  std::cout << "utc_time: " << std::setprecision(8) << utc_time << std::endl;

  return valid_flag;
}