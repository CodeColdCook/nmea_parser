#pragma once

#include <cstdint>
#include <string>

/**
 * @brief utils of parsing data
 */
class UtilsParse {
public:
    /**
     * @brief Converts a buffer containing 8 bytes into a double.
     * @param {uint8_t*} _buffer
     */
    static double ParseDouble(const uint8_t* _buffer);

    /**
     * @brief Parses a string containing a floating-point number into a double.
     * @param {string&} string
     * @param {double&} value
     */
    static bool ParseDouble(const std::string& string, double& value);

    /**
     * @brief Converts a buffer containing 4 bytes into a float.
     * @param {uint8_t*} _buffer
     */
    static float ParseFloat(const uint8_t* _buffer);

    /**
     * @brief Parses a string containing a floating-point number into a float.
     * @param {string&} string
     * @param {float&} value
     */
    static bool ParseFloat(const std::string& string, float& value);

    /**
     * @brief Converts a buffer containing 2 bytes into a signed 16-bit int.
     * @param {uint8_t*} _buffer
     */
    static int16_t ParseInt16(const uint8_t* _buffer);

    /**
     * @brief Converts a buffer containing 4 bytes into a signed 32-bit int.
     * @param {uint8_t*} _buffer
     */
    static int32_t ParseInt32(const uint8_t* _buffer);

    /**
     * @brief Parses a string containing an integer number into an int32_t.
     * @param {string&} string
     * @param {int32_t&} value
     * @param {int32_t} base
     */
    static bool ParseInt32(const std::string& string, int32_t& value, int32_t base = 10);

    /**
     * @brief Converts a buffer containing 4 bytes into an unsigned 32-bit int.
     * @param {uint8_t*} _buffer
     */
    static uint32_t ParseUInt32(const uint8_t* _buffer);

    /**
     * @brief Parses a string containing an integer number into a uint32_t.
     * @param {string&} string
     * @param {uint32_t&} value
     * @param {int32_t} base
     */
    static bool ParseUInt32(const std::string& string, uint32_t& value, int32_t base = 10);

    /**
     * @brief Converts DMS(Degree-Minute-Second) format into an degree
     * @param {double} _dms
     */
    static double ConvertDmsToDegrees(double _dms);
};