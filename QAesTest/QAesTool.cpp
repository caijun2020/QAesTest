#include "QAesTool.h"
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>

#define QAES_ENABLE_DEBUG   1

QAesTool::QAesTool(QObject *parent) :
    QObject(parent),
    m_encryption(nullptr),
    m_key("ilovewuxi2024"),
    m_iv("")
{
    initEncryption();
    hashKey = QCryptographicHash::hash(m_key.toUtf8(), QCryptographicHash::Md5); // must be 16 bytes for AES-128
    hashIV = QCryptographicHash::hash(m_iv.toUtf8(), QCryptographicHash::Md5); // Used for CBC/CFB/OFB mode
}

QAesTool::~QAesTool()
{
    if(nullptr != m_encryption)
    {
        delete m_encryption;
    }
}

QAesTool* QAesTool::instance()
{
    static QAesTool singleton;
    return &singleton;
}

QString QAesTool::encodeString(const QString input, bool base64Flag)
{
    QString encodeStr;
    encodeStr.clear();

    QByteArray encodedText = m_encryption->encode(input.toUtf8(), hashKey, hashIV);

    // convert to Base64
    if(base64Flag)
    {
        encodedText = encodedText.toBase64();
    }

    encodeStr = QString::fromLatin1(encodedText);

#ifdef QAES_ENABLE_DEBUG
    //qDebug() << __FUNCTION__ << " input.toUtf8():" << input.toUtf8();
    //qDebug() << __FUNCTION__ << " encodedText:" << encodedText;
    qDebug() << __FUNCTION__ << " input:" << input << " encodeStr:" << encodeStr;
#endif

    return encodeStr;
}

QString QAesTool::decodeString(const QString input, bool base64Flag)
{
    QString decodeStr;
    decodeStr.clear();

    QByteArray decodedText;

    if(base64Flag)
    {
        // Input QString to QByteArray with fromBase64()
        decodedText = m_encryption->decode(QByteArray::fromBase64(input.toLatin1()), hashKey, hashIV);
    }
    else
    {
        decodedText = m_encryption->decode(input.toLatin1(), hashKey, hashIV);
    }

    decodedText = QAESEncryption::RemovePadding(decodedText, m_padding);

    // Original string is in Utf8, so here convert from Utf8
    decodeStr = QString::fromUtf8(decodedText);

#ifdef QAES_ENABLE_DEBUG
    //qDebug() << __FUNCTION__ << " decodedText:" << decodedText;
    qDebug() << __FUNCTION__ << " input:" << input << " decodeStr:" << decodeStr;
#endif

    return decodeStr;
}

QByteArray QAesTool::encodeString(const QByteArray input, bool base64Flag)
{
    QByteArray ret;
    ret.clear();

    ret = m_encryption->encode(input, hashKey, hashIV);

    // convert to Base64
    if(base64Flag)
    {
        ret = ret.toBase64();
    }

#ifdef QAES_ENABLE_DEBUG
    qDebug() << __FUNCTION__ << " input:" << input << " encodedText:" << ret;
#endif

    return ret;
}

QByteArray QAesTool::decodeString(const QByteArray input, bool base64Flag)
{
    QByteArray ret;
    ret.clear();

    if(base64Flag)
    {
        // Input QByteArray with fromBase64()
        ret = m_encryption->decode(QByteArray::fromBase64(input), hashKey, hashIV);
    }
    else
    {
        ret = m_encryption->decode(input, hashKey, hashIV);
    }

    ret = QAESEncryption::RemovePadding(ret, m_padding);

#ifdef QAES_ENABLE_DEBUG
    qDebug() << __FUNCTION__ << " input:" << input << " decodedText:" << ret;
#endif

    return ret;
}

bool QAesTool::encodeFile(const QString inFilePath, QString outFilePath)
{
    bool ret = false;

    QFile inFile(inFilePath);
    if(inFile.open(QIODevice::ReadOnly))
    {
        QByteArray inputData = inFile.read(inFile.bytesAvailable());
        inFile.close();

        QByteArray data = encodeString(inputData);

        // If output file is empty, by default create one in input file path
        if(outFilePath.isEmpty())
        {
            outFilePath = inFilePath;
            outFilePath.append(".encode");
        }

        QFile outFile(outFilePath);
        if(outFile.open(QIODevice::ReadWrite))
        {
            outFile.write(data);
            outFile.close();
            ret = true;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool QAesTool::decodeFile(const QString inFilePath, QString outFilePath)
{
    bool ret = false;

    QFile inFile(inFilePath);
    if(inFile.open(QIODevice::ReadOnly))
    {
        QByteArray inputData = inFile.read(inFile.bytesAvailable());
        inFile.close();

        QByteArray data = decodeString(inputData);

        // If output file is empty, by default create one in input file path
        if(outFilePath.isEmpty())
        {
            outFilePath = inFilePath;
            outFilePath.append(".decode");
        }

        QFile outFile(outFilePath);
        if(outFile.open(QIODevice::ReadWrite))
        {
            outFile.write(data);
            outFile.close();
            ret = true;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool QAesTool::initKey(QString key)
{
    bool ret = false;
    if(!key.isEmpty())
    {
        m_key = key;
        hashKey = QCryptographicHash::hash(m_key.toUtf8(), QCryptographicHash::Md5); // must be 16 bytes for AES-128
        ret = true;
    }

    return ret;
}

QString QAesTool::getKey() const
{
    return m_key;
}

QString QAesTool::getHashKey() const
{
    return hashKey;
}

bool QAesTool::initIV(QString iv)
{
    bool ret = false;
    m_iv = iv;
    hashIV = QCryptographicHash::hash(m_iv.toUtf8(), QCryptographicHash::Md5);
    qDebug() << "QAesTool::initIV() m_iv=" << m_iv << " hashIV=" << hashIV;
    ret = true;

    return ret;
}

QString QAesTool::getIV() const
{
    return m_iv;
}

QString QAesTool::getHashIV() const
{
    return hashIV;
}

void QAesTool::initEncryption(QAESEncryption::Aes level,
                    QAESEncryption::Mode mode,
                    QAESEncryption::Padding padding)
{

    m_level = level;
    m_mode = mode;
    m_padding = padding;

    if(nullptr != m_encryption)
    {
        delete m_encryption;
    }

    m_encryption = new QAESEncryption(m_level, m_mode, m_padding);
}

QAESEncryption::Aes QAesTool::getAesLevel() const
{
    return m_level;
}

QAESEncryption::Mode QAesTool::getAesMode() const
{
    return m_mode;
}

QAESEncryption::Padding QAesTool::getAesPadding() const
{
    return m_padding;
}
