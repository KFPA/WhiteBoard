#ifndef FILEINFOS_H
#define FILEINFOS_H


#include <string>
#include <vector>

namespace wb {

static const char* FILE_INFO_FILE_INFOS = "fileInfos";
static const char* FILE_INFO_NAME  = "name";
static const char* FILE_INFO_URI = "uri";
static const char* FILE_INFO_ORIGINAL_URI = "originalUri";
static const char* FILE_INFO_TYPE = "type";
static const char* FILE_INFO_USER_NAME = "userName";
static const char* FILE_INFO_METTING_ID = "meetingId";


    struct FileInfo{
            std::string name;
            std::string uri;
            std::string originalUri;
            int32_t type;

            FileInfo() : type(1){}
    };


    class FileInfos
    {
    public:
        FileInfos();
        ~FileInfos();
    public:
        void test();
    public:
        std::string json();
    private:
        int32_t                  m_nMettingId;       //!< 会议id
        std::string              m_strUserName;      //!< 用户名
        std::vector<FileInfo>    m_vecFileInfos;     //!< 文件信息


    };

}

#endif // FILEINFOS_H
