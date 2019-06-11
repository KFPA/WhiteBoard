#ifndef DATA_DEFINE_H
#define DATA_DEFINE_H


#include <string>

namespace wb {

    struct fileInfo{
            std::string name;
            std::string url;
            std::string originalUrl;
            int32_t type;

            fileInfo() : type(1){};
    };

    class FileInfos{

    };

}

#endif // DATA_DEFINE_H
