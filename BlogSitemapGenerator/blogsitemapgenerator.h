#ifndef BLOGSITEMAPGENERATOR_H
#define BLOGSITEMAPGENERATOR_H

#include <QtCore>

class BlogItem
{
public:
    QString url;
    QString title;
    QList<QString> tags;
};

class BlogSitemapGenerator
{
public:
    BlogSitemapGenerator();

    ///< @brief 解析dirPath文件夹下的md文件，存入_data中.
    void readBlogSitemap(const QString &dirPath);

    ///< @brief 生成 Blog的Site Map.
    void writeSitemap(const QString &dirPath);

private:
    void readBlogItem(const QFileInfo &info);

private:
    // QString: tag
    QMap<QString, QList<BlogItem>> _data;
};

#endif // BLOGSITEMAPGENERATOR_H
