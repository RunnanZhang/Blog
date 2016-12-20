#include "blogsitemapgenerator.h"
#include "setting.h"

#include <QCoreApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Setting set("./config.xml");
    QString dir = set.value("dirPath").toString();

    BlogSitemapGenerator b;
    b.readBlogSitemap(dir);
    b.writeSitemap(dir);

    return 0;
}
