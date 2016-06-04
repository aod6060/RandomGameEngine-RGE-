#include "sys.h"


std::string util_loadFile(std::string fn) {
	std::ifstream in(fn.c_str());
	std::stringstream ss;
	std::string temp;
	while (std::getline(in, temp)) {
		ss << temp << std::endl;
	}
	in.close();
	return ss.str();
}

void util_loadFile(std::string fn, std::vector<std::string>& out) {
	std::ifstream in(fn.c_str());
	std::string temp;
	while (std::getline(in, temp)) {
		out.push_back(temp);
	}
	in.close();
}

void util_strSplit(std::string str, char delim, std::vector<std::string>& out) {
	std::stringstream ss(str);
	std::string temp;
	while (std::getline(ss, temp, delim)) {
		out.push_back(temp);
	}
}

int util_toInt(std::string value) {
	return atoi(value.c_str());
}

float util_toFloat(std::string value) {
	return atof(value.c_str());
}

double util_toDouble(std::string value) {
	return atof(value.c_str());
}

bool util_toBool(std::string value) {
	return (value == "true") ? true : false;
}

glm::vec2 util_jsonToVec2(Json::Value& value) {
	glm::vec2 v = glm::vec2(
		value[0].asFloat(),
		value[1].asFloat()
	);
	return v;
}

glm::vec3 util_jsonToVec3(Json::Value& value) {
	glm::vec3 v = glm::vec3(
		value[0].asFloat(),
		value[1].asFloat(),
		value[2].asFloat()
	);
	return v;
}

glm::vec4 util_jsonToVec4(Json::Value& value) {
	glm::vec4 v = glm::vec4(
		value[0].asFloat(),
		value[1].asFloat(),
		value[2].asFloat(),
		value[3].asFloat()
	);
	return v;
}
