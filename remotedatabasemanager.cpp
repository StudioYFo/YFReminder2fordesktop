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
//�յ���ʱoauth_token��secret�������
void RemoteDataBaseManager::reqTknReplyFinished()
{
    int ret=mReqTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mReqTknReply->readAll();

    if(CORRECT_RESULT==ret)//������ȷ
    {
        procTmpToken(mTmpToken,mTmpTokenSecret,getBuf);//��ȡmTmpTokenSecret mTmpToken
        setAuthorise(mTmpToken);//���м�Ȩ
    }
    else
    {
        //showErrInfo(ret,getBuf);
        qWarning("reqTknReplyFinished ERR");
    }
    mReqTknReply->deleteLater();
}

//��ʼ��
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
//���������ļ�
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
        //�޸�Դ�봦
        mAuthoriseOk=true;
    }
}
//��ȡ��ʱtoken��secret
void RemoteDataBaseManager::procTmpToken(QString &tmpToken, QString &tmpTokenSecret, const QByteArray &buf)
{
    QList<QByteArray> list=buf.simplified().split('"');
    tmpTokenSecret=list.at(TMP_TOKEN_SECRET_INDEX);
    tmpToken=list.at(TMP_TOKEN_INDEX);
}
//�û���֤token ��½��Ȩ
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
//�ж���Ȩ���
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
//��ʼAccessToken
void RemoteDataBaseManager::sendAccessToken(bool logInRet)
{
    if(logInRet)//�û�����Ȩ
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
     //   statusBar()->showMessage(tr("�û�δ��Ȩ!"),5000);
        //ui->lblResult2->setText("�û�δ��Ȩ");
    }
}
//acessToken���������
void RemoteDataBaseManager::acesTknReplyFinished()
{
    int ret=mAcesTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mAcesTknReply->readAll();
    if(CORRECT_RESULT==ret)//������ȷ
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
//��ȡoauth_token oauth_token_secret charged_dir
void RemoteDataBaseManager::procAuToken(struAuToken &struTkn, const QByteArray &buf)
{
    QList<QByteArray> list=buf.simplified().split('"');
    struTkn.auTokenSecret=list.at(AUTHOR_TOKEN_SECRET_INDEX);
    struTkn.auToken=list.at(AUTHOR_TOKEM_INDEX);
    struTkn.usrDirID=list.at(AUTHOR_USR_DIR);
    QString tmpUsrId=list.at(AUTHOR_USR_ID).trimmed();
    struTkn.usrID=tmpUsrId.mid(2,tmpUsrId.length()-3);

    //д�������ļ�
    writeCfg();
    reqUsrInfo();//get usr info
}

//д�������ļ�
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
//��ȡ�û���Ϣ
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
//�û���Ϣ�Ѿ�ȫ����ȡ���
void RemoteDataBaseManager::usrInfoReplyFinished()
{
    int ret=mUsrInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUsrInfoReply->readAll();
    if(CORRECT_RESULT==ret)//������ȷ
    {
        procUsrInfo(mUsrInfoStru,getBuf);
        //setQuotaBar();//������������
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult2->setText("�û���Ϣ�Ѿ�ȫ����ȡ���--ERR");
    }
    mUsrInfoReply->deleteLater();
}
//�洢�û���Ϣ���ṹ����
void RemoteDataBaseManager::procUsrInfo(struUsrInfo &usrInfo, const QByteArray &buf)
{
    QList<QByteArray> list=buf.split('"');
    QString tmpStr;//ȥ����һλ�ո�Ȼ��ȥ2~��len-3��λ
    tmpStr=list.at(USR_MAX_FILE_SIZE_INDEX).trimmed();//": 314572800, "
    usrInfo.maxFileSize=tmpStr.mid(2,tmpStr.length()-3);

    tmpStr=list.at(USR_TOTAL_SIZE_INDEX).trimmed();
    usrInfo.quotaTotal=tmpStr.mid(2,tmpStr.length()-3);

    tmpStr=list.at(USR_USED_SIZE_INDEX).trimmed();
    usrInfo.quotaUsed=tmpStr.mid(2,tmpStr.length()-3);
    usrInfo.usrName=list.at(USR_NAME_INDEX);
}
//���������ٷֱȽ���
void RemoteDataBaseManager::setQuotaBar()
{
    double curVal,maxVal;
    QString totalUsed;
    QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
    cfgReader->setIniCodec("UTF-8");
    cfgReader->setValue("OAuthInfo/usrName",mUsrInfoStru.usrName);//д���û����������ļ�

    curVal=(mUsrInfoStru.quotaUsed.toLongLong()/BYTE_TO_MB_UNIT)/1024.00;

    totalUsed.append(QString::number(curVal,'f',2));
    totalUsed.append("G/");
    maxVal=(mUsrInfoStru.quotaTotal.toLongLong()/BYTE_TO_MB_UNIT)/1024.00;
    totalUsed.append(QString::number(maxVal,'f',2));
    totalUsed.append("G");
    //ui->lblResult3->setText(QString::number(curVal*100));
}
//���� reqToken url
QString RemoteDataBaseManager::buildReqTknUrl(QString &consKeyStr, QString &consKeySecretStr)
{
    QString para;
    QString _reqTokenUrl=REQUEST_TOKEN_SRC_URL;
    _reqTokenUrl.append("?");
    _reqTokenUrl.append(OASIGNATURE);
    _reqTokenUrl.append(getReqTknSignature(para,consKeyStr,consKeySecretStr));//���ǩ��
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _reqTokenUrl;
}
//����accessToken url
QString RemoteDataBaseManager::buildAcesTknUrl(QString &consKeyStr, QString &consKeySecretStr, QString &tmpTknStr, QString &tmpTknSercetStr)
{
    QString para;
    QString _acesTknUrl=ACCESS_TOKEN_SRC_URL;
    _acesTknUrl.append("?");
    _acesTknUrl.append(OASIGNATURE);
    _acesTknUrl.append(getAcesTknSignature(para,consKeyStr,consKeySecretStr,\
                                           tmpTknStr,tmpTknSercetStr));
    //���ǩ��
    _acesTknUrl.append("&");
    _acesTknUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
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
    //���ǩ��
    _usrInfoUrl.append("&");
    _usrInfoUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _usrInfoUrl;
}
//����reqToken ��ȡǩ��
//  oauth_consumer_key
//  oauth_nonce
//  oauth_timestamp
QString RemoteDataBaseManager::getReqTknSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr)
{
    QString ReqTokenBaseUrl;
    ReqTokenBaseUrl.clear(); //�������URI·������URL����
    ReqTokenBaseUrl.append("GET&");
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(REQUEST_TOKEN_SRC_URL));
    ReqTokenBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    return  hmacSha1(consKeySecretStr.append("&").toAscii(),ReqTokenBaseUrl.toAscii()); //����ǩ��
}
//����accessToken��ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString RemoteDataBaseManager::getAcesTknSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &tmpTknStr, QString &tmpTknSercetStr)
{
    QString acesTknBaseUrl;
    acesTknBaseUrl.clear(); //�������URI·������URL����
    acesTknBaseUrl.append("GET&");
    acesTknBaseUrl.append(QUrl::toPercentEncoding(ACCESS_TOKEN_SRC_URL));
    acesTknBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);
    paraStr.append(tmpTknStr);

    acesTknBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr.append("&");
    secret=secret.append(tmpTknSercetStr);
    return  hmacSha1(secret.toAscii(),acesTknBaseUrl.toAscii()); //����ǩ��
}
//�����ȡ�û���Ϣ ��ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString RemoteDataBaseManager::getUsrInfoSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString getUsrInfoBaseUrl;
    getUsrInfoBaseUrl.clear(); //�������URI·������URL����
    getUsrInfoBaseUrl.append("GET&");
    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(GET_USR_INFO_URL));
    getUsrInfoBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��

    QString secret=consKeySecretStr.append("&");//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getUsrInfoBaseUrl.toAscii()); //����ǩ��
}
//��ӳ��ò���
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
//-----------------------------------------�ϴ��ļ�
//�����ϴ��ļ�(�Ȼ�ȡ�ϴ��ڵ�)
void RemoteDataBaseManager::reqUploadFile()
{
    //��ȡ�ϴ��ڵ�
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
    else //�ѻ�ȡ�ϴ��ڵ�
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
        buildMultiPart(data);//ʹ��multipart��ʽ�ϴ� ����multipart��
        mUploadFileReply=manager->post(uploadRequest,data);//�ϴ�
        connect(mUploadFileReply,SIGNAL(finished()),this,SLOT(upLoadFileReplyFinished()));
        connect(mUploadFileReply,SIGNAL(uploadProgress(qint64,qint64)),\
                this,SLOT(showUploadProgress(qint64,qint64)));
    }
}
//��ȡ���ϴ��ڵ�
void RemoteDataBaseManager::getUploadLocateFinished()
{
    int ret=mUploadLocateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadLocateReply->readAll();
    mUploadLocateReply->deleteLater();
    if(CORRECT_RESULT==ret)
    {
        QList<QByteArray> list=getBuf.split('"');

        mUploadNode=list.at(3);//�ϴ��ڵ�
        QSettings *cfgReader = new QSettings(cfgFile.fileName(), QSettings::IniFormat);
        cfgReader->setIniCodec("UTF-8");
        cfgReader->setValue("OAuthInfo/uploadNode",mUploadNode);//д���ϴ��ڵ㵽�����ļ�
        reqUploadFile();
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult->setText("getUploadLocateFinished ERR");
    }
}
//�ϴ��ļ� �������
void RemoteDataBaseManager::upLoadFileReplyFinished()
{
    int ret=mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadFileReply->readAll();
    if(CORRECT_RESULT==ret)//������ȷ
    {
        //statusBar()->showMessage(tr("�ϴ��ɹ�!"),2000);
        //ui->lblResult3->setText("Upload success");
    }
    else
    {
        //showErrInfo(ret,getBuf);
        //ui->lblResult3->setText("Upload fail");
    }
    mUploadFileReply->deleteLater();
}
//�ϴ�������
void RemoteDataBaseManager::showUploadProgress(qint64 bytesWrite, qint64 totalBytes)
{
    //ui->lblResult3->setText(QString::number(bytesWrite/totalBytes));
    if(bytesWrite==totalBytes) emit uploadDataBaseFileComplete();
}
//�����ȡ�ϴ��ӿڵ�ַ��url
QString RemoteDataBaseManager::buildUploadLocateUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString para;
    QString _locateUrl=UPLOAD_LOCATE_URL;
    _locateUrl.append("?");
    _locateUrl.append(OASIGNATURE);
    _locateUrl.append(getUploadLocateSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //���ǩ��
    _locateUrl.append("&");
    _locateUrl.append(para);//����ޱ���Ĳ���
    return _locateUrl;
}
//�����ϴ��ļ���url
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
    //���ǩ��
    _sendFileUrl.append("&");
    _sendFileUrl.append(para);//����ޱ���Ĳ���
    return _sendFileUrl;
}
//�����ϴ��ļ��� ǩ��
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
    uploadFileBaseUrl.clear(); //�������URI·������URL����
    uploadFileBaseUrl.append("POST&");
    uploadFileBaseUrl.append(QUrl::toPercentEncoding(fullUrl));
    uploadFileBaseUrl.append("&");


    //�Բ��������������� Ȼ��ϲ�
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

    uploadFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr.append("&");//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),uploadFileBaseUrl.toAscii()); //����ǩ��
}
//����multipart
void RemoteDataBaseManager::buildMultiPart(QByteArray &data)
{
    QByteArray needToUploadFile;
    QString localPath=LOCALUPLOADPATH;
    if(openFile(needToUploadFile,localPath)<=0)
    {
        QMessageBox::warning(this,tr("����"),tr("�޷����ļ�")+localPath);
        return ;
    }

    QString crlf="\r\n";
    QString boundary="--------Michael"+getRandNonce();
    QString contentType="multipart/form-data; boundary="+boundary; //��ʼ��ͷ����Ϣ

    QString endBoundary=crlf+"--"+boundary+"--"+crlf; //������

    boundary="--"+boundary+crlf; //ÿ�����ݿ�ͷ
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
//�����ȡ�ϴ���ַ��ǩ��
QString RemoteDataBaseManager::getUploadLocateSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr)
{
    QString upLocaBaseUrl;
    upLocaBaseUrl.clear(); //�������URI·������URL����
    upLocaBaseUrl.append("GET&");
    upLocaBaseUrl.append(QUrl::toPercentEncoding(UPLOAD_LOCATE_URL));
    upLocaBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    upLocaBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr.append("&");//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),upLocaBaseUrl.toAscii()); //����ǩ��
}

//���ļ�
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
//----------------------------------------�����ļ�
//���������ļ�
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
    {   //������ļ�ʧ�ܣ���ɾ��file����ʹfileָ��Ϊ0��Ȼ�󷵻�
        QMessageBox::warning(this,tr("���ش���"),tr("�޷������ļ�"));
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
//������� ��ȡ�ض����ַ
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
                mDwnFileRealReply=manager->get(QNetworkRequest(realDwnFileUrl));//���ض����ַ��������
                connect(mDwnFileRealReply,SIGNAL(readyRead()),this,SLOT(dwnFileRealReadReady()));
                connect(mDwnFileRealReply,SIGNAL(finished()),this,SLOT(dwnFileRealReplyFinished()));
                //connect(mDwnFileRealReply,SIGNAL(downloadProgress(qint64,qint64)), this,SLOT(showDownProgress(qint64,qint64)));
                break;
            }
        }
    }
    mDownloadFileReply->deleteLater();
}
//�ض���� ��������ļ�����
void RemoteDataBaseManager::dwnFileRealReadReady()
{
    int ret=mDwnFileRealReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(CORRECT_RESULT==ret)//������ȷ
    {
        if (dwnFile!=NULL) dwnFile->write(mDwnFileRealReply->readAll());
    }
    else
    {
        qDebug("download error!!");
        //QByteArray errorInfo("���س���");
        //showErrInfo(ret,errorInfo);
        //ui->lblResult3->setText("download fail");
    }
}
//�ض���� �����ļ����
void RemoteDataBaseManager::dwnFileRealReplyFinished()
{
    //QMessageBox::information(this,"notice","download complete��");
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
//���� �����ļ���url
QString RemoteDataBaseManager::buildDownFileUrl(QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, bool &isAppPath, QString &fromPath)
{
    QString para;
    QString _dwnFileUrl=DOWNLOAD_FILE_URL;
    _dwnFileUrl.append("?");
    _dwnFileUrl.append(OASIGNATURE);
    _dwnFileUrl.append(getDownFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fromPath));
    //���ǩ��
    _dwnFileUrl.append("&");
    _dwnFileUrl.append(para);//����ޱ���Ĳ���
    return _dwnFileUrl;
}
//���� �����ļ���ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//path
//root
QString RemoteDataBaseManager::getDownFileSignature(QString &paraStr, QString &consKeyStr, QString &consKeySecretStr, QString &oauTknStr, QString &oauTknSercetStr, bool &isAppPath, QString &fromPath)
{
    QString dwnFileBaseUrl;
    dwnFileBaseUrl.clear(); //�������URI·������URL����
    dwnFileBaseUrl.append("GET&");
    dwnFileBaseUrl.append(QUrl::toPercentEncoding(DOWNLOAD_FILE_URL));
    dwnFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    dwnFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr.append("&");//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),dwnFileBaseUrl.toAscii()); //����ǩ��
}
//�ж��Ƿ���Ҫ������Ȩ�����ڲ�δ���ǵ�Authorise���ڵ�Ӱ�죬
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
