#ifndef REMOTEDATABASEMANAGER_H
#define REMOTEDATABASEMANAGER_H
#include "PublicFunction.h"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTextCodec>
#include <QWebFrame>
#include <QLabel>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
enum EnumStackUI
{
    SW_WEBVIEW_UI=0,
    SW_KUAIPAN_UI
};

typedef struct _auTokenJson
{
    QString auToken;
    QString auTokenSecret;
    QString usrID;
    QString usrDirID;
    QString mUploadNode;
}struAuToken;

typedef struct _usrInfoJson
{
    QString maxFileSize;
    QString quotaTotal;
    QString quotaUsed;
    QString usrName;
}struUsrInfo;

typedef struct _metaDataPara
{
    bool    useDataPara;
   // int     file_limit;
    //默认0  不分页
    int     page;       //int	默认0 - 不分页；和page_size 一起使用；从 1 开始，表示显示第N页的文件
    int     page_size;  //int	默认20；page 不等于 0才有效；表示每页显示的文件内容。 最大值为512
   // QString filter_ext;
    //string 过滤特定扩展名的文件，用英文半角逗号分隔，只能是ascii字符，
    //每一项的长度不超过5，如：jpg,bmp,png,jpeg 会返回包含上述扩展的文档（如1.jpg）。文件夹不会被过滤。请用小写。
    QString sort_by;    //enum(date, name, size)	默认空，不排序。 在分页（page!=0）的时候才有效。
                        //排序依据，time - 修改日期 name - 文件名 size - 文件大小
                        //在前面加上字母"r"的时候代表倒序，如：sort_by = rsize 表示从大到小排序。
   // QString list;
    //string/JSON bool	默认true。当路径指向是文件夹时，会返回其包含的子文件

}struMetaDataPara;

typedef enum EnumTabWidget
{
    TAB_GET_META=0,
    TAB_SHARE,
    TAB_CREATE_NEW,
    TAB_DEL,
    TAB_MOVE_CPOY,
    TAB_UPLOAD,
    TAB_DWNLOAD
}ETabWidget;

namespace Ui {
    class RemoteDataBaseManager;
}
class MainWindow;
class RemoteDataBaseManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteDataBaseManager(MainWindow *parent = 0);
    ~RemoteDataBaseManager();
    bool isAuthorised();
    void startAuthorise();
    void uploadDataBaseFile();
    void downloadDataBaseFile();
signals:
    void logInResult(bool ret);
    void downloadDataBaseFileComplete();
    void uploadDataBaseFileComplete();
private slots:
    void on_scanNetworkConfigurationFinished();

    void sendAccessToken(bool logInRet);
    void acesTknReplyFinished();

    void reqUsrInfo();
    void usrInfoReplyFinished();

    void reqUploadFile();
    void getUploadLocateFinished();
    void upLoadFileReplyFinished();
    void showUploadProgress(qint64 bytesWrite, qint64 totalBytes);

    void reqDownLoadFile();
    void downLoadFileReplyFinished();
    void dwnFileRealReadReady();
    void dwnFileRealReplyFinished();
    void showDownProgress(qint64 bytesRead, qint64 totalBytes);

    void reqTknReplyFinished();
    void getAuthoriseResult(bool isFinished);
    void on_btnBack_clicked();

private:
    QString buildReqTknUrl(QString &consKeyStr,QString &consKeySecretStr);
    QString buildAcesTknUrl(QString &consKeyStr,QString &consKeySecretStr,\
                              QString &tmpTknStr,QString &tmpTknSercetStr);
    QString buildUsrInfoUrl(QString &consKeyStr,QString &consKeySecretStr,\
                            QString &oauTknStr,QString &oauTknSercetStr);

    QString buildUploadLocateUrl(QString &consKeyStr,QString &consKeySecretStr,\
                                 QString &oauTknStr,QString &oauTknSercetStr);
    QString buildUploadFileUrl(QString &consKeyStr,QString &consKeySecretStr,\
                               QString &oauTknStr,QString &oauTknSercetStr,\
                               const bool &isOverWrite, const bool &isAppPath,\
                               const QString &toPath,QString &preStr);
    QString buildDownFileUrl(QString &consKeyStr,QString &consKeySecretStr,\
                             QString &oauTknStr,QString &oauTknSercetStr,\
                             bool &isAppPath,QString &fromPath);
    QString getDownFileSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr,\
                                 QString &oauTknStr,QString &oauTknSercetStr,\
                                 bool &isAppPath,QString &fromPath);
    void    init();
    void    judgeCfgFile();

    void    procTmpToken(QString &tmpToken,QString &tmpTokenSecret,const QByteArray &buf);
    void    setAuthorise(const QString &tmpToken);
    void    procAuToken(struAuToken &struTkn,const QByteArray &buf);
    void    writeCfg();
    void    procUsrInfo(struUsrInfo &usrInfo,const QByteArray &buf);
    void    setQuotaBar();


    QString getReqTknSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr);
    QString getAcesTknSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr,\
                                QString &tmpTknStr,QString &tmpTknSercetStr);
    QString getUsrInfoSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr,\
                                QString &oauTknStr,QString &oauTknSercetStr);

    void    addTknNonceTimes(QString &paraStr,QString &consKeyStr);

    QString getUploadLocateSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr,\
                                     QString &oauTknStr,QString &oauTknSercetStr);
    QString getUploadFileSignature(QString &paraStr,QString &consKeyStr,QString &consKeySecretStr,\
                                   QString &oauTknStr,QString &oauTknSercetStr,\
                                   const bool &isOverWrite,const bool &isAppPath,\
                                   const QString &toPath,QString &fullUrl);
    void    buildMultiPart(QByteArray &data);
    int     openFile(QByteArray &buf,const QString &filePath);
    QString mTmpTokenSecret;
    QString mTmpToken;
    QString mReqTokenSecret;
    QString mUserDirStr;
    QString mUploadNode;
    bool    mIsAppPath;
    QUrl    inputUrl;
    QLabel  *lMsgLabel;
    QFile   cfgFile;
    QNetworkAccessManager *manager;
    QNetworkConfigurationManager *configuraionmanager;
    QNetworkConfiguration *configuration;
    QNetworkRequest       request;
    QNetworkRequest       uploadRequest;
    QNetworkReply         *mReqTknReply;
    QNetworkReply         *mAcesTknReply;
    QNetworkReply         *mUsrInfoReply;
    QNetworkReply         *mMetaDataReply;
    QNetworkReply         *mCreateFolderReply;
    QNetworkReply         *mDelFileReply;
    QNetworkReply         *mMvFileReply;
    QNetworkReply         *mCpFileReply;
    QNetworkReply         *mUploadLocateReply;
    QNetworkReply         *mUploadFileReply;
    QNetworkReply         *mDownloadFileReply;
    QNetworkReply         *mDwnFileRealReply;
    QNetworkReply         *mShareFileReply;
    int     mAuUrlChangeNum;
    bool    mAuthoriseOk;
    struAuToken mAuTokenInfo;
    struUsrInfo mUsrInfoStru;
    QByteArray mDwnFileBuf;
    QFile *dwnFile;

    Ui::RemoteDataBaseManager *ui;
    MainWindow *mainwindow;
};

#endif // REMOTEDATABASEMANAGER_H
