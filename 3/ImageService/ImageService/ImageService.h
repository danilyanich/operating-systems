#pragma once
using namespace std;
namespace ImageService {

	class ImageService
	{
	public:


		SOCKET establishConnection(string*, string*);

		void start();
		void startDownoloadProcess(string* url, int* numlink);
		void workTestEvent();
		string getCurrentTimeString();
		
		unsigned __fastcall dowloadUrlFile(int* pArg);
		unsigned __fastcall runThreadPool(int* pArg);
	};
}
