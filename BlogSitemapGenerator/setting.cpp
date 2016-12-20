#include "setting.h"

#include <QDebug>
#include <QMap>
#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QRect>

Setting::Setting(const QString &filePath, QObject *parent) :
    QObject(parent),
    _isWrite(false)
{
    _fileName = processFile(filePath);
    readXmlFile(_fileName);
}

Setting::~Setting()
{
    if(_isWrite) {
        writeXmlFile(_fileName);
    }
}

QString Setting::processFile(const QString &filePath)
{
    QString path = QDir::toNativeSeparators(filePath);

    // make sure dir exist.
    int lastIndex = path.lastIndexOf(QDir::separator());
    QString dirPath = path.left(lastIndex);
    QDir dir;
    dir.mkpath(dirPath);

    // make sure the type is xml.
    //ProjectAssistant::checkFileFormat(path, "xml");

    return path;
}

bool Setting::writeXmlFile(const QString &fileName)
{
    QDomDocument doc;

    QString instructData = QString("version=\"1.0\" encoding=\"UTF-8\"");
    QDomProcessingInstruction instruction = doc.createProcessingInstruction(QString("xml"), instructData);
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("Hisign-Configuration");
    doc.appendChild(root);

    for(auto item = _map.begin(); item != _map.end(); ++item)
    {
        QDomElement elem = doc.createElement(item.key());
        elem.setAttribute("value", variantToString(item.value()));
        root.appendChild(elem);
    }

    QString xml = doc.toString();

    QFile file(fileName);
    QString errorStr;
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    if(file.write(xml.toUtf8()) == -1) {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool Setting::readXmlFile(const QString &fileName)
{
    QFile file(fileName);
    QString errorStr;
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file, &errorStr)) {
        qWarning() << "Set the content of xml failed:" << errorStr;
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode node = docElem.firstChild();
    while(!node.isNull()) {
        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QString key = e.tagName();
            QVariant value = stringToVariant(e.attribute("value"));
            _map.insertMulti(key, value);
        }
        node = node.nextSibling();
    }

    return true;
}

void Setting::setValue(const QString &key, const QVariant &value)
{
    if(!_isWrite) {
        _isWrite = true;
    }
    _map.insert(key, value);
}

QVariant Setting::value(const QString &key, const QVariant &defaultValue) const
{
    return _map.value(key, defaultValue);
}

void Setting::insert(const QString &key, const QVariant &value)
{
    if(!_isWrite) {
        _isWrite = true;
    }
    _map.insertMulti(key, value);
}

QList<QVariant> Setting::values(const QString &key) const
{
    return _map.values(key);
}

QString Setting::variantToString(const QVariant &v)
{
    QString result;

    switch (v.type()) {
        case QVariant::Invalid:
            result = QLatin1String("@Invalid()");
            break;

        case QVariant::ByteArray: {
            QByteArray a = v.toByteArray();
            result = QLatin1String("@ByteArray(");
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
            break;
        }

        case QVariant::String:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::KeySequence: {
            result = v.toString();
            if (result.startsWith(QLatin1Char('@')))
                result.prepend(QLatin1Char('@'));
            break;
        }
#ifndef QT_NO_GEOM_VARIANT
        case QVariant::Rect: {
            QRect r = qvariant_cast<QRect>(v);
            result += QLatin1String("@Rect(");
            result += QString::number(r.x());
            result += QLatin1Char(' ');
            result += QString::number(r.y());
            result += QLatin1Char(' ');
            result += QString::number(r.width());
            result += QLatin1Char(' ');
            result += QString::number(r.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Size: {
            QSize s = qvariant_cast<QSize>(v);
            result += QLatin1String("@Size(");
            result += QString::number(s.width());
            result += QLatin1Char(' ');
            result += QString::number(s.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Point: {
            QPoint p = qvariant_cast<QPoint>(v);
            result += QLatin1String("@Point(");
            result += QString::number(p.x());
            result += QLatin1Char(' ');
            result += QString::number(p.y());
            result += QLatin1Char(')');
            break;
        }
#endif // !QT_NO_GEOM_VARIANT

        default: {
#ifndef QT_NO_DATASTREAM
            QDataStream::Version version;
            const char *typeSpec;
            if (v.type() == QVariant::DateTime) {
                version = QDataStream::Qt_5_6;
                typeSpec = "@DateTime(";
            } else {
                version = QDataStream::Qt_4_0;
                typeSpec = "@Variant(";
            }
            QByteArray a;
            {
                QDataStream s(&a, QIODevice::WriteOnly);
                s.setVersion(version);
                s << v;
            }

            result = QLatin1String(typeSpec);
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
#else
            Q_ASSERT(!"Setting: Cannot save custom types without QDataStream support");
#endif
            break;
        }
    }

    return result;
}


QVariant Setting::stringToVariant(const QString &s)
{
    if (s.startsWith(QLatin1Char('@'))) {
        if (s.endsWith(QLatin1Char(')'))) {
            if (s.startsWith(QLatin1String("@ByteArray("))) {
                return QVariant(s.midRef(11, s.size() - 12).toLatin1());
            } else if (s.startsWith(QLatin1String("@Variant("))
                       || s.startsWith(QLatin1String("@DateTime("))) {
#ifndef QT_NO_DATASTREAM
                QDataStream::Version version;
                int offset;
                if (s.at(1) == QLatin1Char('D')) {
                    version = QDataStream::Qt_5_6;
                    offset = 10;
                } else {
                    version = QDataStream::Qt_4_0;
                    offset = 9;
                }
                QByteArray a = s.midRef(offset).toLatin1();
                QDataStream stream(&a, QIODevice::ReadOnly);
                stream.setVersion(version);
                QVariant result;
                stream >> result;
                return result;
#else
                Q_ASSERT(!"Setting: Cannot load custom types without QDataStream support");
#endif
#ifndef QT_NO_GEOM_VARIANT
            } else if (s.startsWith(QLatin1String("@Rect("))) {
                QStringList args = splitArgs(s, 5);
                if (args.size() == 4)
                    return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
            } else if (s.startsWith(QLatin1String("@Size("))) {
                QStringList args = splitArgs(s, 5);
                if (args.size() == 2)
                    return QVariant(QSize(args[0].toInt(), args[1].toInt()));
            } else if (s.startsWith(QLatin1String("@Point("))) {
                QStringList args = splitArgs(s, 6);
                if (args.size() == 2)
                    return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
#endif
            } else if (s == QLatin1String("@Invalid()")) {
                return QVariant();
            }

        }
        if (s.startsWith(QLatin1String("@@")))
            return QVariant(s.mid(1));
    }

    return QVariant(s);
}

QStringList Setting::splitArgs(const QString &s, int idx)
{
    int l = s.length();
    Q_ASSERT(l > 0);
    Q_ASSERT(s.at(idx) == QLatin1Char('('));
    Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

    QStringList result;
    QString item;

    for (++idx; idx < l; ++idx) {
        QChar c = s.at(idx);
        if (c == QLatin1Char(')')) {
            Q_ASSERT(idx == l - 1);
            result.append(item);
        } else if (c == QLatin1Char(' ')) {
            result.append(item);
            item.clear();
        } else {
            item.append(c);
        }
    }

    return result;
}
