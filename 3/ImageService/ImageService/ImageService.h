#pragma once
using namespace std;
namespace ImageService {

	class ImageService
	{
	public:

		void start();
		void startDownoloadProcess(string* url, int* numlink);
		void workTestEvent();
		string getCurrentTimeString();
	};
}
