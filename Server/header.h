#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <tinyxml2.h>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>

static bool isValidMac(const std::string& mac);
bool macExists(std::vector<std::pair<std::string, std::string>> existing, std::string mac);
void addMacPassword(const std::string& filePath, const std::string& mac, const std::string& password);

#endif