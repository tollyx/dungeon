#include "Config.h"
#include <cctype>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

bool isFloat(string myString) {
  istringstream iss(myString);
  float f;
  iss >> noskipws >> f; // noskipws considers leading whitespace invalid
                        // Check the entire string was consumed and if either failbit or badbit is set
  return iss.eof() && !iss.fail();
}

bool isInt(string myString) {
  istringstream iss(myString);
  int i;
  iss >> noskipws >> i; // noskipws considers leading whitespace invalid
                        // Check the entire string was consumed and if either failbit or badbit is set
  return iss.eof() && !iss.fail();
}

// trim from start (in place)
static inline void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
}

// trim from end (in place)
static inline void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(string &s) {
    ltrim(s);
    rtrim(s);
}



Config::Config(char* path) {
  this->path = path;
}

void Config::load() {
  strings.clear();
  bools.clear();
  floats.clear();

  ifstream conf(path);
  if (conf.is_open()) {
    string line;
    while (getline(conf, line)) {
      int pos = line.find("=");
      if (pos > 0) {
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1, line.length());
        
        trim(key);
        trim(value);

        if (bools.find(key) != bools.end() || 
            strings.find(key) != strings.end() || 
            ints.find(key) != ints.end() ||
            floats.find(key) != floats.end()) {
          continue;
        }

        if (isInt(value)) {
          int ival = stoi(value);
          ints.insert(pair<string, float>{key, ival});
        }
        else if (isFloat(value)) {
          float fval = stof(value);
          floats.insert(pair<string, float>{key, fval});
        }
        else if (value.find("true") == 0 && value.length() == 4) {
          bools.insert(pair<string, bool>{key, true});
        }
        else if (value.find("false") == 0 && value.length() == 5) {
          bools.insert(pair<string, bool>{key, false});
        }
        else {
          strings.insert(pair<string, string>{key, value});
        }
      }
    }
    conf.close();
  }
}

void Config::save() {
  ofstream conf(path, ios::trunc);
  if (conf.is_open()) {
    vector<string> vec;
    for each (auto it in ints) {
      vec.push_back(it.first + " = " + to_string(it.second));
    }
    for each (auto it in floats) {
      vec.push_back(it.first + " = " + to_string(it.second));
    }
    for each (auto it in bools) {
      string b;
      if (it.second) {
        b = "true";
      }
      else {
        b = "false";
      }
      vec.push_back(it.first + " = " + b);
    }
    for each (auto it in strings) {
      vec.push_back(it.first + " = " + it.second);
    }
    sort(vec.begin(), vec.end());
    for each (auto it in vec) {
      conf << it << "\n";
    }
    conf.close();
  }
}

void Config::deleteValue(string key) {
  strings.erase(key);
  ints.erase(key);
  floats.erase(key);
  bools.erase(key);
}

string Config::getString(string key, string defaultvalue) {
  auto iter = strings.find(key);
  if (iter != strings.end()) {
    return iter->second;
  }
  else {
    deleteValue(key);
    strings.insert(pair<string, string>{key, defaultvalue});
    return defaultvalue;
  }
}

int Config::getInt(string key, int defaultvalue) {
  auto iter = ints.find(key);
  if (iter != ints.end()) {
    return iter->second;
  }
  else {
    deleteValue(key);
    ints.insert(pair<string, int>{key, defaultvalue});
    return defaultvalue;
  }
}

float Config::getFloat(std::string key, float defaultvalue) {
  auto iter = floats.find(key);
  if (iter != floats.end()) {
    return iter->second;
  }
  else {
    deleteValue(key);
    floats.insert(pair<string, float>{key, defaultvalue});
    return defaultvalue;
  }
}

bool Config::getBool(std::string key, bool defaultvalue) {
  auto iter = bools.find(key);
  if (iter != bools.end()) {
    return iter->second;
  }
  else {
    deleteValue(key);
    bools.insert(pair<string, bool>{key, defaultvalue});
    return defaultvalue;
  }
}


Config::~Config() {
}
