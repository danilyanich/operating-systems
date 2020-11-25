#include <iostream>

class Cluster {
private:
	int clusterIndex;
	int clusterSize;
	std::string clusterInfo = "";
	bool clusterBool;
public:
	Cluster(int index, int size) {
		clusterSize = size;
		clusterIndex = index;
		clusterBool = true;
	}

	int getClusterIndex() {
		return clusterIndex;
	}

	bool isFree() {
		return clusterBool;
	}

	void takeCluster() {
		clusterBool = false;
	}

	void freeCluster() {
		clusterBool = true;
	}

	void setClusterInfo(std::string info) {
		clusterInfo = info;
	}

	std::string getClusterInfo() {
		return clusterInfo;
	}

	void freeClusterInfo() {
		clusterInfo = "";
	}
};