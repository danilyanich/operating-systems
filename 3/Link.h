//
// Created by alexandr on 29.12.20.
//

#ifndef INC_3_LINK_H
#define INC_3_LINK_H

#pragma once

#include <string>

using namespace std;
class Link {
public:
    string hostName;
    string path;

    Link(string hostName, string path);

};

Link* convertToImageLink(string url, int* socketAddressNumber, string* imageNameArr);

#endif //INC_3_LINK_H
