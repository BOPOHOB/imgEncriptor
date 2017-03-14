#include "face.h"
#include <QLabel>
#include "filenameedit.h"
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include "encriptor.h"

Face::Face(QWidget *parent)
    : QWidget(parent)
{
    this->setLayout(new QHBoxLayout);
    QLabel* const image(new QLabel(this));
    this->layout()->addWidget(image);
    QWidget* const rightSide(new QWidget);
    this->layout()->addWidget(rightSide);
    rightSide->setLayout(new QVBoxLayout);
    FileNameEdit* const imagePicker(new FileNameEdit(rightSide));
    rightSide->layout()->addWidget(imagePicker);
    QTextEdit* const editor(new QTextEdit(rightSide));
    editor->setText("Мама мыла раму!");
    rightSide->layout()->addWidget(editor);
    QPushButton* save(new QPushButton("Сохранить"));
    rightSide->layout()->addWidget(save);

    auto slot = [image, editor, imagePicker](){
        const QImage img(imagePicker->text());
        if (img.isNull()) {
            return;
        }
        image->setPixmap(QPixmap::fromImage(Encriptor::encript(img.scaled(QSize(500,500), Qt::KeepAspectRatio), editor->toPlainText())));
    };
    connect(imagePicker, &FileNameEdit::fileNameChanged, slot);
    connect(editor, &QTextEdit::textChanged, slot);

    connect(save, &QPushButton::clicked, [image, this]() {
        const QString fName(QFileDialog::getSaveFileName(this, "Где сохранить?", QString(), "*.bmp"));
        if (!fName.isNull()) {
            const QPixmap* const img(image->pixmap());
            img->save(fName, "bmp");
        }
    });
}
