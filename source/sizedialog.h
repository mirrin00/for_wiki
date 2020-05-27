#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

/* This is a window for entering dimensions
 * for create empty image or for collage
 * Modes:
 * 0 -- collage
 * 1 -- empty image
 */

#include <QDialog>
#include <QFileDialog>
#include <QAbstractButton>

namespace Ui {
class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SizeDialog(int mode, QWidget *parent = nullptr);
    ~SizeDialog();

    unsigned int getWidth();

    unsigned int getHeight();

    QString getFileName();

private slots:
    void on_width_valueChanged(int arg1);

    void on_height_valueChanged(int arg1);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    unsigned int width,height;

    QString filename;

    Ui::SizeDialog *ui;

};

#endif // SIZEDIALOG_H
