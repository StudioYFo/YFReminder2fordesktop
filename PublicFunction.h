#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H
#include <QString>
#include <QCryptographicHash>
#include <QUrl>
#include <QTime>
#include <time.h>

const int CORRECT_RESULT            =200;//正确的返回结果

enum EnumTmpTokenIndex
{
    TMP_TOKEN_SECRET_INDEX=3,//返回的临时tokenSecret在reply中的游标
    TMP_TOKEN_INDEX=7       //返回的临时token在reply中的游标
};

enum EnumAuTokenIndex   //authToken
{
    AUTHOR_TOKEN_SECRET_INDEX=3,
    AUTHOR_TOKEM_INDEX=7,
    AUTHOR_USR_ID=10,
    AUTHOR_USR_DIR=13
};

enum EnumUsrInfoIndex   //用户信息
{
    USR_MAX_FILE_SIZE_INDEX=2,
    USR_TOTAL_SIZE_INDEX=4,
    USR_USED_SIZE_INDEX=6,
    USR_NAME_INDEX=9
};


#define CONSUMER_KEY            "xcjr2kncdsDWUv1c"      //consumer_key
#define CONSUMER_SECRET         "CVuxW6dw2J41HW2N"      //consumer_secret

#define REQUEST_TOKEN_SRC_URL   "https://openapi.kuaipan.cn/open/requestToken"
#define AUTHORISE_SRC_URL       "https://www.kuaipan.cn/api.php?ac=open&op=authorise&oauth_token="
#define ACCESS_TOKEN_SRC_URL    "https://openapi.kuaipan.cn/open/accessToken"
#define GET_USR_INFO_URL        "http://openapi.kuaipan.cn/1/account_info"
#define GET_META_DATA_URL       "http://openapi.kuaipan.cn/1/metadata/"
#define CREATE_FOLDER_URL       "http://openapi.kuaipan.cn/1/fileops/create_folder"
#define DEL_FILE_URL            "http://openapi.kuaipan.cn/1/fileops/delete"
#define MOVE_FILE_URL           "http://openapi.kuaipan.cn/1/fileops/move"
#define COPY_FILE_URL           "http://openapi.kuaipan.cn/1/fileops/copy"
#define UPLOAD_LOCATE_URL       "http://api-content.dfs.kuaipan.cn/1/fileops/upload_locate"
#define UPLOAD_FILE_URL         "1/fileops/upload_file"
#define DOWNLOAD_FILE_URL       "http://api-content.dfs.kuaipan.cn/1/fileops/download_file"
#define SHARE_FILE_URL          "http://openapi.kuaipan.cn/1/shares/"

#define OANONCE                   "oauth_nonce="
#define OATIMESTAMP               "oauth_timestamp="
#define OASIGNATURE               "oauth_signature="
#define OACONSUMER_KEY            "oauth_consumer_key="
#define OATOKEN                   "oauth_token="

#define DATA_FILE_LIMIT           "file_limit="
#define DATA_FILTER_EXT           "filter_ext="
#define DATA_LIST                 "list="
#define DATA_PAGE                 "page="
#define DATA_PAGE_SIZE            "page_size="
#define DATA_SORT_BY              "sort_by="

#define FOLDER_NAME               "path="
#define FOLDER_ROOT               "root="

#define TO_RECYLE                 "to_recycle="
#define DATA_FROM_PATH            "from_path="
#define DATA_TO_PATH              "to_path="

#define UPLOAD_OVER_WRITE         "overwrite="

#define BYTE_TO_MB_UNIT           (1024*1024UL)
QString hmacSha1(QByteArray key, QByteArray baseString);
QString getRandNonce();
QString getTimeStamp();
QString removeUrlEncode(const QString &urlStr);


#define TOPATH                      "SavedReminder.txt"
#define LOCALUPLOADPATH                 "SavedReminder.txt"
#define REMOTEDOWNPATH             "SavedReminder.txt"
#define LOCALDOWNPATH               "SavedReminder.txt"
#endif // PUBLICFUNCTION_H
