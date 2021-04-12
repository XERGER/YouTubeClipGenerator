#include "YouTubeClipGenerator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YouTubeClipGenerator w;
    w.show();
    return a.exec();
}
