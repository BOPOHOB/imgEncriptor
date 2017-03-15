#include <QApplication>
#include <QDebug>

#include "face.h"
#include "wav.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Face w;
    w.show();
    return app.exec();
}
