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
    //Ĭ��0  ����ҳ
    int     page;       //int	Ĭ��0 - ����ҳ����page_size һ��ʹ�ã��� 1 ��ʼ����ʾ��ʾ��Nҳ���ļ�
    int     page_size;  //int	Ĭ��20��page ������ 0����Ч����ʾÿҳ��ʾ���ļ����ݡ� ���ֵΪ512
   // QString filter_ext;
    //string �����ض���չ�����ļ�����Ӣ�İ�Ƕ��ŷָ���ֻ����ascii�ַ���
    //ÿһ��ĳ��Ȳ�����5���磺jpg,bmp,png,jpeg �᷵�ذ���������չ���ĵ�����1.jpg�����ļ��в��ᱻ���ˡ�����Сд��
    QString sort_by;    //enum(date, name, size)	Ĭ�Ͽգ������� �ڷ�ҳ��page!=0����ʱ�����Ч��
                        //�������ݣ�time - �޸����� name - �ļ��� size - �ļ���С
                        //��ǰ�������ĸ"r"��ʱ��������磺sort_by = rsize ��ʾ�Ӵ�С����
   // QString list;
    //string/JSON bool	Ĭ��true����·��ָ�����ļ���ʱ���᷵������������ļ�

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
