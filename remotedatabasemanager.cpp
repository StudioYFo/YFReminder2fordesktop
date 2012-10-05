#include "remotedatabasemanager.h"
#include "ui_remotedatabasemanager.h"
#include "mainwindow.h"
RemoteDataBaseManager::RemoteDataBaseManager(MainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::RemoteDataBaseManager)
{
    ui->setupUi(this);
    this->mainwindow=parent;
    init();
    connect(ui->webView,SIGNAL(loadFinished(bool)),SLOT(getAuthoriseResult(bool)));
    connect(this,SIGNAL(logInResult(bool)),this,SLOT(sendAccessToken(bool)));
#if defined(Q_OS_SYMBIAN)
    connect(configuraionmanager,SIGNAL(updateCompleted()),this,SLOT(on_scanNetworkConfigurationFinished()));
    configuraionmanager->updateConfigurations();
#endif
}

RemoteDataBaseManager::~RemoteDataBaseManager()
{
    delete ui;
}
void RemoteDataBaseManager::on_scanNetworkConfigurationFinished(){
#if defined(Q_OS_SYMBIAN)
    QList<QNetworkConfiguration> allConfigurations=configuraionmanager->allConfigurations();
    for(int i=0;i<allConfigurations.length();i++){
        if(QNetworkConfiguration::Discovered==allConfigurations.at(i).state()
                && ("CMNET"==allConfigurations.at(i).name()
                || "CMWAP"==allConfigurations.at(i).name()
                || "3gnet"==allConfigurations.at(i).name()
                || "3gwap"==allConfigurations.at(i).name()))
        {
            this->manager->setConfiguration(allConfigurations.at(i));
            break;
        }
    }
#endif
}
//收到临时oauth_token和secret反馈完成
void RemoteDataBaseManager::reqTknReplyFinished()
{
    int ret=mReqTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mReqTknReply->readAll();

    if(CORRECT_RESULT==ret)//返回正确
    {
        procTmpToken(mTmpToken,mTmpTokenSecret,getBuf);//提取mTmpTokenSecret mTmpToken
        setAuthorise(mTmpToken);//进行鉴权
    }
    else
    {
        //showErrInfo(ret,getBuf);
        qWarning("reqTknReplyFinished ERR");
    }
    mReqTknReply->deleteLater();
}

//初始化
void RemoteDataBaseManager::init()
{
    manager = new QNetworkAccessManager(this);
#if defined(Q_OS_SYMBIAN)
    configuraionmanager=new QNetworkConfigurationManager();
    configuration=new QNetworkConfiguration();
#endif
    mReqTokenSecret.clear();
    mReqTokenSecret.append(CONSUMER_SECRET);
    mReqTokenSecret.append("&");
    mAuUrlChangeNum=0;
    mAuthoriseOk=false;
    mIsAppPath=true;
    judgeCfgFile();
    mUploadNode="NULL";
}
//读入配置文件
void RemoteDataBaseManager::judgeCfgFile()
{
    QString path=QCoreApplication::applicationDirPath().append("/cfg.ini");
    cfgFile.setFileName(path);
    if(cfgFile.exists())
    {
        QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
        cfgReader->setIniCodec("UTF-8");
        mAuTokenInfo.auToken= cfgReader->value("OAuthInfo/oauth_token",QString("NULL")).toString();
        mAuTokenInfo.auTokenSecret=cfgReader->value("OAuthInfo/oauth_token_secret",\
                                                      QString("NULL")).toString();
        mAuTokenInfo.usrID=cfgReader->value("OAuthInfo/user_id",QString("NULL")).toString();
        mAuTokenInfo.usrDirID=cfgReader->value("OAuthInfo/charged_dir",QString("NULL")).toString();

        mUsrInfoStru.usrName=cfgReader->value("OAuthInfo/usrName",QString("NULL")).toString();
        mUploadNode=cfgReader->value("OAuthInfo/UploadNode",QString("NULL")).toString();
        if(mUsrInfoStru.usrName!="NULL")
        {
        }
        //修改源码处
        mAuthoriseOk=true;
    }
}
//提取临时token和secret
void RemoteDataBaseManager::procTmpToken(QString &tmpToken, QString &tmpTokenSecret, const QByteArray &buf)
{
    QList<QByteArray> list=buf.simplified().split('"');
    tmpTokenSecret=list.at(TMP_TOKEN_SECRET_INDEX);
    tmpToken=list.at(TMP_TOKEN_INDEX);
}
//用户验证token 登陆授权
void RemoteDataBaseManager::setAuthorise(const QString &tmpToken)
{
    QString authorizeUrl;
    authorizeUrl.append(AUTHORISE_SRC_URL);
    authorizeUrl.append(tmpToken);
    inputUrl.setUrl(authorizeUrl);
    request.setUrl(inputUrl);
    mAuUrlChangeNum=0;
    mAuthoriseOk=false;
    ui->webView->load(request);
}
//判断授权结果
void RemoteDataBaseManager::getAuthoriseResult(bool isFinished)
{
    if(isFinished)
    {
        mAuUrlChangeNum++;
        if(2==mAuUrlChangeNum)
        {
            if(ui->webView->findText("ERROR_OAUTH_TOKEN"))
            {
                mAuthoriseOk=false;
                //ui->lblResult->setText("OAUTH TOKEN ERR");
            }
            else
            {
                mAuthoriseOk=true;
                //ui->swUi->setCurrentIndex(SW_KUAIPAN_UI);
                //ui->lblResult->setText("OAUTH TOKEN OK");
            }
            emit logInResult(mAuthoriseOk);
        }
    }
}
//开始AccessToken
void RemoteDataBaseManager::sendAccessToken(bool logInRet)
{
    if(logInRet)//用户已授权
    {
        QString ConsumerKey=QString(CONSUMER_KEY);
        QString ConsumerSecret=QString(CONSUMER_SECRET);
        QString acesTknUrl=buildAcesTknUrl(ConsumerKey,ConsumerSecret,\
                                           mTmpToken,mTmpTokenSecret);
        inputUrl.setEncodedUrl(acesTknUrl.toAscii());
        request.setUrl(inputUrl);
        mAcesTknReply=manager->get(request);
        connect(mAcesTknReply, SIGNAL(finished()),this,SLOT(acesTknReplyFinished()));
    }
    else
    {
     //   statusBar()->showMessage(tr("用户未授权!"),5000);
        //ui->lblResult2->setText("用户未授权");
    }
}
//acessToken包返回完成
void RemoteDataBaseManager::acesTknReplyFinished()
{
    int ret=mAcesTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mAcesTknReply->readAll();
    if(CORRECT_RESULT==ret)//返回正确
    {
        //ui->lblResult2->setText("LOGON OK");
        procAuToken(mAuTokenInfo,getBuf);
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult->setText(getBuf);
        //ui->lblResult2->setText("LOGON ERR");
    }
    mAcesTknReply->deleteLater();
}
//提取oauth_token oauth_token_secret charged_dir
void RemoteDataBaseManager::procAuToken(struAuToken &struTkn, const QByteArray &buf)
{
    QList<QByteArray> list=buf.simplified().split('"');
    struTkn.auTokenSecret=list.at(AUTHOR_TOKEN_SECRET_INDEX);
    struTkn.auToken=list.at(AUTHOR_TOKEM_INDEX);
    struTkn.usrDirID=list.at(AUTHOR_USR_DIR);
    QString tmpUsrId=list.at(AUTHOR_USR_ID).trimmed();
    struTkn.usrID=tmpUsrId.mid(2,tmpUsrId.length()-3);

    //写入配置文件
    writeCfg();
    reqUsrInfo();//get usr info
}

//写入配置文件
void RemoteDataBaseManager::writeCfg()
{
    cfgFile.open(QIODevice::WriteOnly);
    cfgFile.close();
    QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
    cfgReader->setIniCodec("UTF-8");
    cfgReader->setValue("OAuthInfo/oauth_token",mAuTokenInfo.auToken);
    cfgReader->setValue("OAuthInfo/oauth_token_secret",mAuTokenInfo.auTokenSecret);
    cfgReader->setValue("OAuthInfo/user_id",mAuTokenInfo.usrID);
    cfgReader->setValue("OAuthInfo/charged_dir",mAuTokenInfo.usrDirID);
    cfgReader->setValue("OAuthInfo/usrName","NULL");
    cfgReader->setValue("OAuthInfo/uploadNode","NULL");
}
//获取用户信息
void RemoteDataBaseManager::reqUsrInfo()
{
    QString ConsumerKey=QString(CONSUMER_KEY);
    QString ConsumerSecret=QString(CONSUMER_SECRET);
    QString usrInfoUrl=buildUsrInfoUrl(ConsumerKey,ConsumerSecret,\
                                       mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret);
    inputUrl.setEncodedUrl(usrInfoUrl.toAscii());
    request.setUrl(inputUrl);
    mUsrInfoReply=manager->get(request);
    connect(mUsrInfoReply, SIGNAL(finished()),this,SLOT(usrInfoReplyFinished()));
}
//用户信息已经全部获取完毕
void RemoteDataBaseManager::usrInfoReplyFinished()
{
    int ret=mUsrInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUsrInfoReply->readAll();
    if(CORRECT_RESULT==ret)//返回正确
    {
        procUsrInfo(mUsrInfoStru,getBuf);
        //setQuotaBar();//更新容量界面
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult2->setText("用户信息已经全部获取完毕--ERR");
    }
    mUsrInfoReply->deleteLater();
}
//存储用户信息到结构体中
void RemoteDataBaseManager::procUsrInfo(struUsrInfo &usrInfo, const QByteArray &buf)
{
    QList<QByteArray> list=buf.split('"');
    QString tmpStr;//去后面一位空格，然后去2~（len-3）位
    tmpStr=list.at(USR_MAX_FILE_SIZE_INDEX).trimmed();//": 314572800, "
    usrInfo.maxFileSize=tmpStr.mid(2,tmpStr.length()-3);

    tmpStr=list.at(USR_TOTAL_SIZE_INDEX).trimmed();
    usrInfo.quotaTotal=tmpStr.mid(2,tmpStr.length()-3);

    tmpStr=list.at(USR_USED_SIZE_INDEX).trimmed();
    usrInfo.quotaUsed=tmpStr.mid(2,tmpStr.length()-3);
    usrInfo.usrName=list.at(USR_NAME_INDEX);
}
//设置容量百分比界面
void RemoteDataBaseManager::setQuotaBar()
{
    double curVal,maxVal;
    QString totalUsed;
    QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
    cfgReader->setIniCodec("UTF-8");
    cfgReader->setValue("OAuthInfo/usrName",mUsrInfoStru.usrName);//写入用户名到配置文件

    curVal=(mUsrInfoStru.quotaUsed.toLongLong()/BYTE_TO_MB_UNIT)/1024.00;

    totalUsed.append(QString::number(curVal,'f',2));
    totalUsed.append("G/");
    maxVal=(mUsrInfoStru.quotaTotal.toLongLong()/BYTE_TO_MB_UNIT)/1024.00;
    totalUsed.append(QString::number(maxVal,'f',2));
    totalUsed.append("G");
    //ui->lblResult3->setText(QString::number(curVal*100));
}
//构建 reqToken url
QString RemoteDataBaseManager::buildReqTknUrl(QString &consKeyStr, QString &consKeySecretStr)
{
    QString para;
    QString _reqTokenUrl=REQUEST_TOKEN_SRC_URL;
    _reqTokenUrl.append("?");
    _reqTokenUrl.append(OASIGNATURE);
    _reqTokenUrl.append(getReqTknSignature(para,consKeyStr,consKeySecretStr));//添加签名
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _reqTokenUrl;
}
//构造accessToken url
QString RemoteDataBaseManager::buildAcesTknUrl(QString &consKeyStr, QString &consKeySecretStr, QString &tmpTknStr, QString &tmpTknSercetStr)
{
    QString para;
    QString _acesTknUrl=ACCESS_TOKEN_SRC_URL;
    _acesTknUrl.append("?");
    _acesTknUrl.append(OASIGNATURE);
    _acesTknUrl.append(getAcesTknSignature(para,consKeyStr,consKeySecretStr,\
                                           tmpTknStr,tmpTknSercetStr));
    //添加签名
    _acesTknUrl.append("&");
    _acesTknUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _acesTknUrl;
}

QString RemoteDataBaseManager::buildUsrInfoUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString para;
    QString _usrInfoUrl=GET_USR_INFO_URL;
    _usrInfoUrl.append("?");
    _usrInfoUrl.append(OASIGNATURE);
    _usrInfoUrl.append(getUsrInfoSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //添加签名
    _usrInfoUrl.append("&");
    _usrInfoUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _usrInfoUrl;
}
//构造reqToken 获取签名
//  oauth_consumer_key
//  oauth_nonce
//  oauth_timestamp
QString RemoteDataBaseManager::getReqTknSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr)
{
    QString ReqTokenBaseUrl;
    ReqTokenBaseUrl.clear(); //将请求的URI路径进行URL编码
    ReqTokenBaseUrl.append("GET&");
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(REQUEST_TOKEN_SRC_URL));
    ReqTokenBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    return  hmacSha1(consKeySecretStr.append("&").toAscii(),ReqTokenBaseUrl.toAscii()); //返回签名
}
//构造accessToken的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString RemoteDataBaseManager::getAcesTknSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &tmpTknStr, QString &tmpTknSercetStr)
{
    QString acesTknBaseUrl;
    acesTknBaseUrl.clear(); //将请求的URI路径进行URL编码
    acesTknBaseUrl.append("GET&");
    acesTknBaseUrl.append(QUrl::toPercentEncoding(ACCESS_TOKEN_SRC_URL));
    acesTknBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);
    paraStr.append(tmpTknStr);

    acesTknBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr.append("&");
    secret=secret.append(tmpTknSercetStr);
    return  hmacSha1(secret.toAscii(),acesTknBaseUrl.toAscii()); //返回签名
}
//构造获取用户信息 的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString RemoteDataBaseManager::getUsrInfoSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString getUsrInfoBaseUrl;
    getUsrInfoBaseUrl.clear(); //将请求的URI路径进行URL编码
    getUsrInfoBaseUrl.append("GET&");
    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(GET_USR_INFO_URL));
    getUsrInfoBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串

    QString secret=consKeySecretStr.append("&");//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getUsrInfoBaseUrl.toAscii()); //返回签名
}
//添加常用参数
void RemoteDataBaseManager::addTknNonceTimes(QString &paraStr, QString &consKeyStr)
{
    paraStr.append(OACONSUMER_KEY); //cosumer_key
    paraStr.append(consKeyStr);
    paraStr.append("&");
    paraStr.append(OANONCE);        //oauth_nonce
    paraStr.append(getRandNonce());
    paraStr.append("&");
    paraStr.append(OATIMESTAMP);    //oauth_timestamp
    paraStr.append(getTimeStamp());
}
//-----------------------------------------上传文件
//请求上传文件(先获取上传节点)
void RemoteDataBaseManager::reqUploadFile()
{
    //获取上传节点
    if("NULL"==mUploadNode)
    {
        QString ConsumerKey=QString(CONSUMER_KEY);
        QString ConsumerSecret=QString(CONSUMER_SECRET);
        QString uploadLocUrl=buildUploadLocateUrl(ConsumerKey,ConsumerSecret,\
                                                  mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret);
        inputUrl.setEncodedUrl(uploadLocUrl.toAscii());
        request.setUrl(inputUrl);
        mUploadLocateReply=manager->get(request);
        connect(mUploadLocateReply,SIGNAL(finished()),this,SLOT(getUploadLocateFinished()));
    }
    else //已获取上传节点
    {
        QByteArray data;
        QString toPath=TOPATH;//ui->tRemoteUploadPath->text();
        QString ConsumerKey=QString(CONSUMER_KEY);
        QString ConsumerSecret=QString(CONSUMER_SECRET);
        bool isOverWrite=true;
        bool isAppPath=true;
        QString uploadFileUrl=buildUploadFileUrl(ConsumerKey,ConsumerSecret,\
                                                 mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                                 isOverWrite,isAppPath,toPath,mUploadNode);

        inputUrl.setEncodedUrl(uploadFileUrl.toAscii());
        uploadRequest.setUrl(inputUrl);
        buildMultiPart(data);//使用multipart方式上传 构造multipart包
        mUploadFileReply=manager->post(uploadRequest,data);//上传
        connect(mUploadFileReply,SIGNAL(finished()),this,SLOT(upLoadFileReplyFinished()));
        connect(mUploadFileReply,SIGNAL(uploadProgress(qint64,qint64)),\
                this,SLOT(showUploadProgress(qint64,qint64)));
    }
}
//获取到上传节点
void RemoteDataBaseManager::getUploadLocateFinished()
{
    int ret=mUploadLocateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadLocateReply->readAll();
    mUploadLocateReply->deleteLater();
    if(CORRECT_RESULT==ret)
    {
        QList<QByteArray> list=getBuf.split('"');

        mUploadNode=list.at(3);//上传节点
        QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
        cfgReader->setIniCodec("UTF-8");
        cfgReader->setValue("OAuthInfo/uploadNode",mUploadNode);//写入上传节点到配置文件
        reqUploadFile();
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult->setText("getUploadLocateFinished ERR");
    }
}
//上传文件 反馈完毕
void RemoteDataBaseManager::upLoadFileReplyFinished()
{
    int ret=mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadFileReply->readAll();
    if(CORRECT_RESULT==ret)//返回正确
    {
        //statusBar()->showMessage(tr("上传成功!"),2000);
        //ui->lblResult3->setText("Upload success");
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult3->setText("Upload fail");
    }
    mUploadFileReply->deleteLater();
}
//上传进度条
void RemoteDataBaseManager::showUploadProgress(qint64 bytesWrite, qint64 totalBytes)
{
    //ui->lblResult3->setText(QString::number(bytesWrite/totalBytes));
    if(bytesWrite==totalBytes) emit uploadDataBaseFileComplete();
}
//构造获取上传接口地址的url
QString RemoteDataBaseManager::buildUploadLocateUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString para;
    QString _locateUrl=UPLOAD_LOCATE_URL;
    _locateUrl.append("?");
    _locateUrl.append(OASIGNATURE);
    _locateUrl.append(getUploadLocateSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //添加签名
    _locateUrl.append("&");
    _locateUrl.append(para);//添加无编码的参数
    return _locateUrl;
}
//构建上传文件的url
QString RemoteDataBaseManager::buildUploadFileUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, const bool &isOverWrite, const bool &isAppPath, const QString &toPath, QString &preStr)
{
    QString para;
    QString fullUploadFileUrl=preStr.append(UPLOAD_FILE_URL);
    QString _sendFileUrl=fullUploadFileUrl;
    _sendFileUrl.append("?");
    _sendFileUrl.append(OASIGNATURE);
    _sendFileUrl.append(getUploadFileSignature(para,consKeyStr,consKeySecretStr,\
                                                 oauTknStr,oauTknSercetStr,\
                                               isOverWrite,isAppPath,\
                                               toPath,fullUploadFileUrl));
    //添加签名
    _sendFileUrl.append("&");
    _sendFileUrl.append(para);//添加无编码的参数
    return _sendFileUrl;
}
//构建上传文件的 签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//overwrite
//path
//root
QString RemoteDataBaseManager::getUploadFileSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, const bool &isOverWrite, const bool &isAppPath, const QString &toPath, QString &fullUrl)
{
    QString uploadFileBaseUrl;
    uploadFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    uploadFileBaseUrl.append("POST&");
    uploadFileBaseUrl.append(QUrl::toPercentEncoding(fullUrl));
    uploadFileBaseUrl.append("&");


    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(UPLOAD_OVER_WRITE); //overwrite
    if(isOverWrite)
        paraStr.append("True");
    else paraStr.append("False");

    paraStr.append("&");
    paraStr.append(FOLDER_NAME);       //path
    paraStr.append(QUrl::toPercentEncoding(toPath));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);        //root
    if(isAppPath)
    {
        paraStr.append("app_folder");
    }
    else
    {
        paraStr.append("kuaipan");
    }

    uploadFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr.append("&");//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),uploadFileBaseUrl.toAscii()); //返回签名
}
//构造multipart
void RemoteDataBaseManager::buildMultiPart(QByteArray &data)
{
    QByteArray needToUploadFile;
    QString localPath=LOCALUPLOADPATH;
    if(openFile(needToUploadFile,localPath)<=0)
    {
        QMessageBox::warning(this,tr("错误"),tr("无法打开文件")+localPath);
        return ;
    }

    QString crlf="\r\n";
    QString boundary="--------Michael"+getRandNonce();
    QString contentType="multipart/form-data; boundary="+boundary; //开始的头部信息

    QString endBoundary=crlf+"--"+boundary+"--"+crlf; //结束处

    boundary="--"+boundary+crlf; //每行数据开头
    QByteArray bond=boundary.toAscii();

    data.append(bond);


    data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\""\
                        +localPath+"\""+crlf).toAscii());
    data.append(QString("Content-Type: application/octet-stream"+crlf).toAscii());
    data.append(crlf.toAscii());
    data.append(needToUploadFile);
    data.append(endBoundary.toAscii());
    uploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
    uploadRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());
}
//构造获取上传地址的签名
QString RemoteDataBaseManager::getUploadLocateSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString upLocaBaseUrl;
    upLocaBaseUrl.clear(); //将请求的URI路径进行URL编码
    upLocaBaseUrl.append("GET&");
    upLocaBaseUrl.append(QUrl::toPercentEncoding(UPLOAD_LOCATE_URL));
    upLocaBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    upLocaBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr.append("&");//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),upLocaBaseUrl.toAscii()); //返回签名
}

//打开文件
int RemoteDataBaseManager::openFile(QByteArray &buf, const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        buf=file.readAll();
    }
    file.close();
    return buf.size();
}
//----------------------------------------下载文件
//请求下载文件
void RemoteDataBaseManager::reqDownLoadFile()
{
    mDwnFileBuf.clear();
    bool isAppPath=true;
    QString ConsumerKey=QString(CONSUMER_KEY);
    QString ConsumerSecret=QString(CONSUMER_SECRET);
    QString fromPath=REMOTEDOWNPATH;
    dwnFile=new QFile(LOCALDOWNPATH);
    //ui->barDownFile->setValue(0);
    //ui->lblResult2->setText(QString::number(0));

    if(!dwnFile->open(QIODevice::WriteOnly))
    {   //如果打开文件失败，则删除file，并使file指针为0，然后返回
        QMessageBox::warning(this,tr("下载错误"),tr("无法创建文件"));
        delete dwnFile;
        dwnFile =NULL;
        return;
    }
    QString downLoadFileUrl=buildDownFileUrl(ConsumerKey,ConsumerSecret,\
                                             mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                             isAppPath,fromPath);
    inputUrl.setEncodedUrl(downLoadFileUrl.toAscii());
    request.setUrl(inputUrl);
    mDownloadFileReply=manager->get(request);
    connect(mDownloadFileReply,SIGNAL(finished()),this,SLOT(downLoadFileReplyFinished()));
}
//反馈完毕 获取重定向地址
void RemoteDataBaseManager::downLoadFileReplyFinished()
{
    QVariant returnCode = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(301==returnCode.toInt() ||  302==returnCode.toInt())
    {
        for(int i=0;i<mDownloadFileReply->rawHeaderPairs().count();i++)
        {
            //qDebug()<<"Pairs()"<<mDownloadFileReply->rawHeaderPairs().at(i) ;
            if(mDownloadFileReply->rawHeaderPairs().at(i).first == "Location")
            {
                QUrl realDwnFileUrl=QUrl(mDownloadFileReply->rawHeaderPairs().at(i).second);
                mDwnFileRealReply=manager->get(QNetworkRequest(realDwnFileUrl));//用重定向地址重新下载
                connect(mDwnFileRealReply,SIGNAL(readyRead()),this,SLOT(dwnFileRealReadReady()));
                connect(mDwnFileRealReply,SIGNAL(finished()),this,SLOT(dwnFileRealReplyFinished()));
                //connect(mDwnFileRealReply,SIGNAL(downloadProgress(qint64,qint64)), this,SLOT(showDownProgress(qint64,qint64)));
                break;
            }
        }
    }
    mDownloadFileReply->deleteLater();
}
//重定向后 获得下载文件反馈
void RemoteDataBaseManager::dwnFileRealReadReady()
{
    int ret=mDwnFileRealReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(CORRECT_RESULT==ret)//返回正确
    {
        if (dwnFile!=NULL) dwnFile->write(mDwnFileRealReply->readAll());
    }
    else
    {
        qDebug("download error!!");
        //QByteArray errorInfo("下载出错");
        //showErrInfo(ret,errorInfo);
        //ui->lblResult3->setText("download fail");
    }
}
//重定向后 下载文件完毕
void RemoteDataBaseManager::dwnFileRealReplyFinished()
{
    //QMessageBox::information(this,"notice","download complete！");
    mDwnFileRealReply->deleteLater();
    dwnFile->flush();
    dwnFile->close();
    delete dwnFile;
    dwnFile=NULL;
    emit downloadDataBaseFileComplete();
}

void RemoteDataBaseManager::showDownProgress(qint64 bytesRead, qint64 totalBytes)
{
    //ui->lblResult2->setText(QString::number(bytesRead/totalBytes));
    if(bytesRead==totalBytes) emit downloadDataBaseFileComplete();
}
//构造 下载文件的url
QString RemoteDataBaseManager::buildDownFileUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, bool &isAppPath, QString &fromPath)
{
    QString para;
    QString _dwnFileUrl=DOWNLOAD_FILE_URL;
    _dwnFileUrl.append("?");
    _dwnFileUrl.append(OASIGNATURE);
    _dwnFileUrl.append(getDownFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fromPath));
    //添加签名
    _dwnFileUrl.append("&");
    _dwnFileUrl.append(para);//添加无编码的参数
    return _dwnFileUrl;
}
//构造 下载文件的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//path
//root
QString RemoteDataBaseManager::getDownFileSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, bool &isAppPath, QString &fromPath)
{
    QString dwnFileBaseUrl;
    dwnFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    dwnFileBaseUrl.append("GET&");
    dwnFileBaseUrl.append(QUrl::toPercentEncoding(DOWNLOAD_FILE_URL));
    dwnFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(FOLDER_NAME);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder");
    }
    else
    {
        paraStr.append("kuaipan");
    }

    dwnFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr.append("&");//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),dwnFileBaseUrl.toAscii()); //返回签名
}
//判断是否需要重新授权，现在并未考虑到Authorise过期的影响，
bool RemoteDataBaseManager::isAuthorised()
{
    return mAuthoriseOk;
}

void RemoteDataBaseManager::startAuthorise()
{
    QString ConsumerKey=QString(CONSUMER_KEY);
    QString ConsumerSecret=QString(CONSUMER_SECRET);
    QString reqTokenUrl=buildReqTknUrl(ConsumerKey,ConsumerSecret);
    inputUrl.setEncodedUrl(reqTokenUrl.toAscii());
    request.setUrl(inputUrl);
    mReqTknReply = manager->get(request);
    connect(mReqTknReply, SIGNAL(finished()),this,SLOT(reqTknReplyFinished()));
}

void RemoteDataBaseManager::uploadDataBaseFile()
{
    reqUploadFile();
}

void RemoteDataBaseManager::downloadDataBaseFile()
{
    reqDownLoadFile();
}



void RemoteDataBaseManager::on_btnBack_clicked()
{
    this->mainwindow->show();
    this->hide();
}
