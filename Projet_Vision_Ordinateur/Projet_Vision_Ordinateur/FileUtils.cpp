#include "FileUtils.hpp"

std::string filesPath(int argc, char** argv)
{
	if(argc >= 2)
	{
		return argv[1];
	}
	else
		return "";
}

void createFilePath(std::string &filename, int fileIndex, std::string filePath)
{
	filename = "00" + std::to_string(fileIndex) + ".jpg";
	if(filename.length() > 7)
	{
		filename.erase(0, 1);
	}
	if(filePath != "")
		filename = filePath + "\\" + filename;
}

void writeResultOnFile(int fileIndex, std::string filePath, bool response[20][4])
{
	std::ofstream resultFile;

	std::string filename = "00" + std::to_string(fileIndex) + ".cases";
	if(filename.length() > 7)
	{
		filename.erase(0, 1);
	}
	if(filePath != "")
		filename = filePath + "\\" + filename;

	resultFile.open(filename);
	for(int i = 0; i < 20; i++)
	{
		resultFile << i << " ";
		for(int j = 0; j < 4; j++)
		{
			if(response[i][j])
				resultFile << "X";
			else
				resultFile << "O";
		}
		resultFile << std::endl;
	}
	resultFile.close();
}