#include <QApplication>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>

#include "encriptor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QImage img(QFileDialog::getOpenFileName(0, "что открыть", QString(), "*.bmp"));
    QMessageBox::information(nullptr, "извлеченные данные", Encriptor::decript(img));
    return a.exec();
}
