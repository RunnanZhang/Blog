#ifndef ZRN_SETTING_H
#define ZRN_SETTING_H

#include <QObject>
#include <QVariant>
#include <QMap>

class Setting : public QObject
{
    Q_OBJECT

public:
    Setting(const QString &filePath, QObject *parent = Q_NULLPTR);
    ~Setting();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void insert(const QString &key, const QVariant &value);
    QList<QVariant> values(const QString &key) const;

private:
    bool readXmlFile(const QString &fileName);
    bool writeXmlFile(const QString &fileName);

    ///< @brief parser functions.
    QString variantToString(const QVariant &v);
    QVariant stringToVariant(const QString &s);
    QStringList splitArgs(const QString &s, int idx);
    ///< @brief make sure dir exist. make sure this file is the type of xml.
    QString processFile(const QString &filePath);

private:
    QMap<QString, QVariant> _map;
    QString _fileName;

    // write file after setValue call.
    bool _isWrite;

};

#endif // ZRN_SETTING_H
