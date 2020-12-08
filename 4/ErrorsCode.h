//
// Created by shund on 25.11.2017.
//

#ifndef FILESYSTEM_ERRORSCODE_H
#define FILESYSTEM_ERRORSCODE_H


class ErrorsCode {

public:
    static const short CREATE_FILE_ERROR = 1;
    static const short CREATE_FILE_MAPPING_ERROR = 2;
    static const short MAP_VIEW_OF_FILE_ERROR = 3;
    static const short SUCCESSFUL_IMPLEMENTATION = 0;

    static const short SUCCESSFUL_WRITE = 5;
    static const short LACK_OF_MEMORY = -1;
    static const short WRONG_FILE_NAME = -2;
    static const short FILE_NOT_FOUND = -3;
    static const short FILE_ALREADY_EXIST = -4;
};
#endif //FILESYSTEM_ERRORSCODE_H
