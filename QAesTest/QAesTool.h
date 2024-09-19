#ifndef QAESTOOL_H
#define QAESTOOL_H

#include <QObject>
#include "qaesencryption.h"


class QAesTool : public QObject
{
    Q_OBJECT
public:
    explicit QAesTool(QObject *parent = nullptr);
    virtual ~QAesTool();

    /*-----------------------------------------------------------------------
    FUNCTION:		instance
    PURPOSE:		Get a QAesTool instance
    ARGUMENTS:		None
    RETURNS:		Return a static QAesTool pointer
    -----------------------------------------------------------------------*/
    static QAesTool* instance();

    // Encode/Decode QString
    QString encodeString(const QString input, bool base64Flag = true);
    QString decodeString(const QString input, bool base64Flag = true);

    QByteArray encodeString(const QByteArray input, bool base64Flag = true);
    QByteArray decodeString(const QByteArray input, bool base64Flag = true);

    // Encode/Decode file
    bool encodeFile(const QString inFilePath, QString outFilePath = "");
    bool decodeFile(const QString inFilePath, QString outFilePath = "");

    bool initKey(QString key);
    QString getKey() const;
    QString getHashKey() const;

    bool initIV(QString iv);
    QString getIV() const;
    QString getHashIV() const;

    QAESEncryption::Aes getAesLevel() const;
    QAESEncryption::Mode getAesMode() const;
    QAESEncryption::Padding getAesPadding() const;

    // Init AES encryption level/mode/padding
    void initEncryption(QAESEncryption::Aes level = QAESEncryption::AES_128,
                        QAESEncryption::Mode mode = QAESEncryption::ECB,
                        QAESEncryption::Padding padding = QAESEncryption::PKCS7);

private:
    QAESEncryption *m_encryption;
    QString m_key;
    QString m_iv;
    QByteArray hashKey;
    QByteArray hashIV;

    QAESEncryption::Aes m_level;
    QAESEncryption::Mode m_mode;
    QAESEncryption::Padding m_padding;

signals:

};

#endif // QAESTOOL_H
