#pragma once
#include <string>
using namespace std;

	class ImageLink {

	public:
		string hostName;
		string path;

		ImageLink(string hostName, string path);
		ImageLink* convertToImageLink(std::string url, int* socketAddressNumber, std::string* imageNameArr);
	};




