#include "ImageLink.h"
#include <regex>

const char* pathImageRegex = "http(s)?:\/\/((.+))";


ImageLink::ImageLink(string hostName, string path)
{
	this->hostName = hostName;
	this->path = path;
}

ImageLink* ImageLink::convertToImageLink(std::string url, int* socketAddressNumber, std::string* imageNameArr)
{
	char temp[1000];

	regex pathPattern(pathImageRegex);
	string addressUrl = regex_replace(url, pathPattern, "$1");

	url = addressUrl;

	(*socketAddressNumber)++;
	imageNameArr[*socketAddressNumber] = url;

	for (int j = 0; j < url.length(); j++) {
		temp[j] = url[j];
		if (temp[j] == '/') {
			imageNameArr[*socketAddressNumber][j] = '.';
		}
		else if (temp[j] == '\\') {
			continue;
		}
		else {
			imageNameArr[*socketAddressNumber][j] = url[j];
		}

	}
	return new ImageLink(hostName,path);
}

