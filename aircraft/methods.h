#pragma once
#include "Model.h"
#include <string>


class methods
{
public:
	static std::string getDirectory(std::string path);
	static ModelPtr loadMesh(std::string path);
};
