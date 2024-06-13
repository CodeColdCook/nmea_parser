#include <gtest/gtest.h>

#include "nmea_parser.h"

#ifndef SET_CB_FUNCTION_NAME
#define SET_CB_FUNCTION_NAME(name) Set##name##Callback
#endif

void SET_CB_FUNCTION_NAME(Foo)() {
    // 生成的回调函数名称将是 SetFooCallback
    // 这里可以添加回调函数的实现
    std::cout << "hanshu mingc " << std::endl;
}

TEST(NmeaParser, GPGGA) {
  NmeaParser paser;
  std::string gpgga_msg =
      "$GPGGA,014434.70,3817.13334637,N,12139.72994196,E,4,07,1.5,6.571,M,8."
      "942,M,0.7,0016*79";
  double time = 1920.56;
  NmeaType r = paser.ParseNmeaMessage(gpgga_msg, time);
  EXPECT_EQ(r, NmeaType::NMEA_GPGGA);
}

TEST(NmeaParser, GPGST) {
  NmeaParser paser;
  std::string msg =
      "$GPGST,203017.00,1.25,0.02,0.01,-16.7566,0.02,0.01,0.03*7D";
  double time = 1920.56;
  NmeaType r = paser.ParseNmeaMessage(msg, time);
  EXPECT_EQ(r, NmeaType::NMEA_GPGST);
}

TEST(NmeaParser, GPGLL) {
  NmeaParser paser;
  std::string msg =
      "$GPGLL,5109.0262317,N,11401.8407304,W,202725.00,A,D*79";
  double time = 1920.56;
  NmeaType r = paser.ParseNmeaMessage(msg, time);
  EXPECT_EQ(r, NmeaType::NMEA_GPGLL);
}

int main(int argc, char **argv) {
  SetFooCallback();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
