#pragma once
#include <string>
#include <map>

class Config {
  char* path;
  std::map<std::string, std::string> strings;
  std::map<std::string, float> floats;
  std::map<std::string, int> ints;
  std::map<std::string, bool> bools;
public:
  Config(char* path);
  void load();
  void save();
  void deleteValue(std::string key);
  std::string getString(std::string key, std::string defaultvalue);
  int getInt(std::string key, int defaultvalue);
  float getFloat(std::string key, float defaultvalue);
  bool getBool(std::string key, bool defaultvalue);
  ~Config();
};

