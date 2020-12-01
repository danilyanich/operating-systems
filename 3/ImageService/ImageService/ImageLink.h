#pragma once
#include <string>
using namespace std;

	class ImageLink {

	public:
		string hostName;
		string path;

		ImageLink(string hostName, string path);
		ImageLink* convertToImageLink(string url, int* socketAddressNumber, string* imageNameArr);
	};




