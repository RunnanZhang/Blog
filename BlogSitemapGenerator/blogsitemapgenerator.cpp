#include "blogsitemapgenerator.h"

BlogSitemapGenerator::BlogSitemapGenerator()
{

}

void BlogSitemapGenerator::readBlogSitemap(const QString &dirPath)
{
    QDir dir(dirPath);
    QFileInfoList infoList = dir.entryInfoList(QDir::Files);

    for(auto info : infoList) {
        readBlogItem(info);
    }
}

void BlogSitemapGenerator::readBlogItem(const QFileInfo &info)
{
    if(info.suffix() != QLatin1String("md")) {
        return;
    }

    QFile file(info.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Read file failed : " << info.absoluteFilePath();
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    int i = 0;
    BlogItem item;
    item.url = QLatin1Char('/') + info.baseName();

    // 读取文件前6行，解析出标题和标签.
    while (i++ < 6) {
        QString line = in.readLine();
        int index;

        if((index = line.indexOf("title:")) >= 0) {
            QString title = line.mid(index + 6);
            item.title = title.trimmed();
        } else if((index = line.indexOf("tags:")) >= 0) {
            QString tags = line.mid(index + 5);
            tags = tags.trimmed();

            // tags: [Qt, windows], we use any sequence of non-word characters as the separator.
            item.tags = tags.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        }
    }

    // 加入整体以tag为key的map中.
    for(auto tag : item.tags) {
        // 两种情况都包含，若没有此tag，则返回默认构造的list.
        QList<BlogItem> blogList = _data.value(tag);
        blogList.append(item);
        _data.insert(tag, blogList);
    }

    file.close();
    qDebug() << "analyze" << info.absoluteFilePath() << "complete.";
}

void BlogSitemapGenerator::writeSitemap(const QString &dirPath)
{
    QFile file(dirPath + QDir::separator() + QLatin1String("sitemap.md"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Write sitemap file failed at : " << dirPath;
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << "---" << endl
        << "title: All Documents" << endl
        << "date: 2015-12-21 08:08:08" << endl
        << "tags: Index" << endl
        << "---" << endl << endl;

    for(auto i = _data.constBegin(); i != _data.constEnd(); ++i) {
        out << endl << "## " << i.key() << endl;

        QList<BlogItem> list = i.value();
        for(auto item : list) {
            out << QString("* [%1](%2)").arg(item.title).arg(item.url) << endl;
        }
    }

    file.close();
    qDebug() << "generate sitemap success.";
}
