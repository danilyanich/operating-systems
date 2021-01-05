#pragma once
#include <string>
class ImageLink {
public: std::string hostName;
public: std::string imagePath;

	  ImageLink(std::string hostName, std::string imagePath);
};

ImageLink* convertToImageLink(std::string url, int* socketAddressNumber, std::string* imageNameArr);