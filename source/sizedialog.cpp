#include "sizedialog.h"
#include "ui_sizedialog.h"

SizeDialog::SizeDialog(int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);
    switch (mode) {
    case 0:
        ui->buttonBox->addButton(QDialogButtonBox::Open);
        ui->label_width->setText(tr("Count OX"));
        ui->label_height->setText(tr("Count OY"));
        break;
    case 1:
        ui->buttonBox->addButton(QDialogButtonBox::Ok);
        break;
    }
}

SizeDialog::~SizeDialog()
{
    delete ui;
}

void SizeDialog::on_width_valueChanged(int arg1)
{
    width=arg1;
}

void SizeDialog::on_height_valueChanged(int arg1)
{
    height=arg1;
}

unsigned int SizeDialog::getWidth(){
    return width;
}

unsigned int SizeDialog::getHeight(){
    return height;
}

QString SizeDialog::getFileName(){
    return filename;
}

void SizeDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button)!=QDialogButtonBox::Open) return;
    QFileDialog open_dialog(this);
    open_dialog.setAcceptMode(QFileDialog::AcceptOpen);
    open_dialog.setFileMode(QFileDialog::AnyFile);
    open_dialog.setViewMode(QFileDialog::Detail);
    open_dialog.setModal(true);
    QStringList list;
    if(open_dialog.exec())
        list=open_dialog.selectedFiles();
    if(list.length()>0) filename=list[0];
}
