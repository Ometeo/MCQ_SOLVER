#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>;
#include <fstream>

std::string filesPath(int argc, char** argv);
void createFilePath(std::string &filename, int fileIndex, std::string filePath);

void writeResultOnFile(int fileIndex, std::string filePath, bool response[20][4]);

#endif