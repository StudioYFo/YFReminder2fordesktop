
#include"PublicFunction.h"

//hmacSha1�㷨ʵ��
QString hmacSha1(QByteArray key, QByteArray baseString)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    //return  hashed.toBase64();//ԭ��Ĳ�����url�����

    QString signatureStr=hashed.toBase64(); //���ﷵ��url����
    return QUrl::toPercentEncoding(signatureStr);
}


//��������� oauth_nonce
QString getRandNonce()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}

//���ص�ǰʱ��
QString getTimeStamp()
{
    time_t curTime=time(0);
    return QString().number(curTime);
}

//ȥ��url����
QString removeUrlEncode(const QString &urlStr)
{
    QUrl url;
    url.setEncodedUrl(urlStr.toAscii());
    return url.toString();
}
