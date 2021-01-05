#include"ImageLink.h"
#include <regex>
#include <string>
using namespace std;

const char* pathRegex = "http(s)?://([^/?:]+)(/?.+)";
const char* hostRegex = "http(s)?://([^/?:]+)/?.+";
const char* nameRegex = "http(s)?://((.+))";

ImageLink* convertToImageLink(string url, int* socketAddressNumber, string* imageNameArr) {

	char temp[100];

	regex pathPattern(pathRegex);
	string imagePath = regex_replace(url, pathPattern, "$3");

	regex hostPattern(hostRegex);
	string hostName = regex_replace(url, hostPattern, "$2");

	regex namePattern(nameRegex);
	string addressUrl = regex_replace(url, namePattern, "$2");

	url = addressUrl;

	(*socketAddressNumber)++;
	imageNameArr[*socketAddressNumber] = url;
	int lenght = url.length();

	for (int j = 0; j < lenght; j++) {
		temp[j] = url[j];
		if (temp[j] == '/') {
			imageNameArr[*socketAddressNumber][j] = '.';
		}
		else {
			imageNameArr[*socketAddressNumber][j] = url[j];
		}
	}
	return new ImageLink(hostName, imagePath);
}

ImageLink::ImageLink(std::string hostName, std::string imagePath)
{
	this->hostName = hostName;
	this->imagePath = imagePath;
}