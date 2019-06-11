#include "fileinfos.h"
#include "QJsonObject"
#include "QJsonArray"
#include "QJsonDocument"
#include "utils.h"
#include "QDebug"

namespace wb {



    FileInfos::FileInfos() : m_nMettingId(0)
    {

    }

    FileInfos::~FileInfos()
    {

    }

    void FileInfos::test()
    {
        FileInfo info;
        info.name = "wpppp";
        info.originalUri ="wppp";
        info.uri = "wppp";
        m_vecFileInfos.push_back(info);

        m_nMettingId = 112;
        m_strUserName = "wppp";


    }

    std::string FileInfos::json()
    {
        std::string strJson;
        QJsonObject json;
        json.insert(FILE_INFO_METTING_ID,m_nMettingId);
        json.insert(FILE_INFO_USER_NAME,m_strUserName.c_str());

        QJsonObject obj;
        for(auto itor = m_vecFileInfos.begin();itor != m_vecFileInfos.end(); ++itor){

            obj.insert(FILE_INFO_NAME,itor->name.c_str());

            obj.insert(FILE_INFO_URI,itor->uri.c_str());

            obj.insert(FILE_INFO_ORIGINAL_URI,itor->originalUri.c_str());

            obj.insert(FILE_INFO_TYPE,itor->type);
        }

        json.insert(FILE_INFO_FILE_INFOS,obj);
        strJson = Utils::qJsonObj2qStr(json).toStdString();
        return strJson;
    }

}

