#include "Link.h"
#include <regex>
#include <string>
using namespace std;

Link::Link(string hostName, string path)
{
    this->hostName = hostName;
    this->path = path;
}

Link* convertToImageLink(string url, int* socketAddressNumber, string* imageNameArr)
{
	char temp[200];
	
	const char* path = "http(s)?:\/\/([^/?:]+)(/?.+)";
	const char* host = "http(s)?:\/\/([^/?:]+)/?.+";
	const char* name = "http(s)?:\/\/((.+))";
	
	regex pathPattern(path);
	string imagePath = regex_replace(url, pathPattern, "$3");

	regex hostPattern(host);
	string hostName = regex_replace(url, pathPattern, "$2");

	regex namePattern(name);
	string addressUrl = regex_replace(url, pathPattern, "$2");

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
	return new Link(hostName, imagePath);
}
