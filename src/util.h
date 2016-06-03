#ifndef UTIL_H
#define UTIL_H


std::string util_loadFile(std::string fn);

void util_loadFile(std::string fn, std::vector<std::string>& out);

void util_strSplit(std::string str, char delim, std::vector<std::string>& out);


#endif