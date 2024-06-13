#include <iostream>
#include <sstream>

std::string StringToHex(const std::string& data) {
  const std::string hex = "0123456789ABCDEF";
  std::stringstream ss;

  for (std::string::size_type i = 0; i < data.size(); ++i)
    ss << hex[(unsigned char)data[i] >> 4] << hex[(unsigned char)data[i] & 0xf];
  std::cout << ss.str() << std::endl;
  return ss.str();
}

std::string HexToStr(const std::string& str) {
  std::string result;
  for (size_t i = 0; i < str.length(); i += 2) {
    std::string byte = str.substr(i, 2);
    char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
    result.push_back(chr);
  }
  return result;
}

int main() {
  std::string result = HexToStr("244244474c4c2c333935342e343130312c4e2c31313933362e363835342c452c3030303130302e33352c562c562a37330d0a");
  std::cout << "str: " << result << std::endl;
  return 0;
}
