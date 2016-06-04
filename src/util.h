#ifndef UTIL_H
#define UTIL_H


std::string util_loadFile(std::string fn);

void util_loadFile(std::string fn, std::vector<std::string>& out);

void util_strSplit(std::string str, char delim, std::vector<std::string>& out);

int util_toInt(std::string value);

float util_toFloat(std::string value);

double util_toDouble(std::string value);

bool util_toBool(std::string value);

glm::vec2 util_jsonToVec2(Json::Value& v);

glm::vec3 util_jsonToVec3(Json::Value& v);

glm::vec4 util_jsonToVec4(Json::Value& v);

#endif