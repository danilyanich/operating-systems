#pragma once

#include <string>
using namespace std;

class Link {
public:
	  string hostName;
	  string path;

	  Link(string hostName, string path);
};

Link* convertToImageLink(std::string url, int* socketAddressNumber, std::string* imageNameArr);