#pragma once

#include <iostream>
#include <map>
#include <memory>

#include "nmea_sentence.h"

/**
 * @brief NMEA information paser
 */
class NmeaParser {
 public:
  typedef std::shared_ptr<NmeaParser> Ptr;

  /**
   * @brief Constuct a new NmeaParser object
   */
  NmeaParser();

  /**
   * @brief Parse NMEA Message
   * @param {string&} _message
   * @param {double} _message_timestamp
   */
  NmeaType ParseNmeaMessage(const std::string& _message,
                            double _message_timestamp = 0);

  // /**
  //  * @brief Get the latest parsed RrkGPS protobuf
  //  */
  // std::vector<SmileSMower::RtkGPS> GetLatestRtkData();

 private:
  /**
   * @brief Searches for a valid ASCII message within a string.
   * @param {string&} _sentense
   * @param {size_t} _current_idx
   * @param {size_t&} _start_idx
   * @param {size_t&} _end_idx
   */
  void FindAsciiSentence(const std::string& _sentense, size_t _current_idx,
                         size_t& _start_idx, size_t& _end_idx);

  /**
   * @brief Finds the location of the next checksum in a valid ASCII sentence.
   * @param {string&} _sentense
   * @param {size_t} _start_idx
   */
  size_t GetCheckSumStart(const std::string& _sentense, size_t _start_idx);

  /**
   * @brief Calculates the checksum of a NMEA sentence.
   * @param {string&} _sentence
   */
  uint8_t NmeaCheckSum(const std::string& _sentence);

  /**
   * @brief Extracts an NMEA sentence from an input string.
   * @param {string&} _str
   * @param {size_t} _start_idx
   * @param {string} _sentence
   */
  int32_t GetNmeaSentence(const std::string& _str, size_t _start_idx,
                          std::string& _sentence);

  /**
   * @brief Takes a sentence extracted by GetNmeaSentence and converts it
   * @param {string&} _sentence
   * @param {NmeaSentence&} _vectorized_message
   */
  void VectorizeNmeaSentence(const std::string& _sentence,
                             NmeaSentence& _vectorized_message);

  /**
   * @brief Extracts messages from a buffer of NMEA message
   * @param {string} _message
   * @param {vector<NmeaSentence>} _nmea_senstence
   */
  bool ExtractCompleteMessages(std::string _message,
                               std::vector<NmeaSentence>& _nmea_senstence,
                               std::string& _remaining);

  /**
   * @brief  Parse GPGGA message
   *
   * @param _sentence
   * @param _gpgga_info
   * @return true
   * @return false
   */
  bool ParseGPGGA(const NmeaSentence& _sentence, GPGGAInfo& _gpgga_info);

  /**
   * @brief Parse GPGST message
   *
   * @param _sentence
   * @param _gpgst_info
   * @return true
   * @return false
   */
  bool ParseGPGST(const NmeaSentence& _sentence, GPGSTInfo& _gpgst_info);

  bool ParseGPGLL(const NmeaSentence& _sentence, GPGLLInfo& _gpgll_info);

 private:
  /// Precedes checkums in ASCII messages
  static const std::string CHECKSUM_FLAG;
  /// Separates data fields in ASCII messages
  static const std::string FIELD_SEPARATOR;
  /// Separates header from body in ASCII NovAtel messages
  static const std::string HEADER_SEPARATOR;
  /// Indicates the beginning of a NMEA sentence
  static const std::string NMEA_SENTENCE_FLAG;
  /// Indicates the end of an NMEA message
  static const std::string NMEA_ENDLINE;

  /// NmeaType-string mapping
  std::map<std::string, NmeaType> nmea_type_string_map_;

  /// Nmea String message
  std::string _nmea_message;

  /// RTK-GPS meassge
  GPGGAInfo gpgga_info;
  GPGSTInfo gpgst_info;
  GPGLLInfo gpgll_info;
};