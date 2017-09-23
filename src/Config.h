#pragma once
#include <string>
#include <map>

class Config {
  std::string path;
  std::map<std::string, std::string> strings;
  std::map<std::string, float> floats;
  std::map<std::string, int> ints;
  std::map<std::string, bool> bools;
public:
  Config(std::string path);
  void load();
  void save();
  void deleteValue(std::string key);
  std::string getString(std::string key, std::string defaultvalue);
  int getInt(std::string key, int defaultvalue);
  float getFloat(std::string key, float defaultvalue);
  bool getBool(std::string key, bool defaultvalue);
  void setString(std::string key, std::string value);
  void setInt(std::string key, int value);
  void setFloat(std::string key, float value);
  void setBool(std::string key, bool value);
  ~Config();
};

