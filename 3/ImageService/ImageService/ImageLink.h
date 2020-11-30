#include <string>
using namespace std;

namespace ImageLink {

	class ImageLink {

	public:
		string hostName;
		string path;

		ImageLink(string hostName, string path);
		ImageLink* convertToImageLink(string url, int* socketAdress, string* imageName);
	};
}