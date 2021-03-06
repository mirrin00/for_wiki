#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new MyScene(ui->graphicsView,this);
    scene->setSize(ui->size_edit->value());
    ui->graphicsView->setScene(scene);
    for(int i=0;i<COLORS_COUNT;i++)
        colors[i]=Qt::white;
    ui->graphicsView->setEnabled(false);
    enabled=false;
    ui->menubar->addAction(tr("Help"),this,SLOT(menuHelp()) );
    background=new QLabel(this);
    background->setPixmap(QPixmap(QString(":/background/background.jpg")));
    background->setGeometry(0,0,0,0);
    background->hide();
    background->lower();
    ui->graphicsView->setStyleSheet(QString("background: transparent"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete background;
}



void MainWindow::on_triangle_radiobutton_toggled(bool checked)
{
    ui->fill_checkbox->setEnabled(checked);
    ui->color_triangle_fill->setEnabled(checked);
    ui->color_triangle->setEnabled(checked);
    if(checked  ){
        ui->color_triangle_fill->setEnabled(ui->fill_checkbox->isChecked());
        scene->setRGBA1(colors[COLORS::TRIANGLE_COLOR].red(),colors[COLORS::TRIANGLE_COLOR].green(),colors[COLORS::TRIANGLE_COLOR].blue(),colors[COLORS::TRIANGLE_COLOR].alpha());
    }
    scene->resetPoints();
    if(checked) scene->setMode(3);
}

void MainWindow::on_line_radiobutton_toggled(bool checked)
{
    ui->color_line->setEnabled(checked);
    scene->resetPoints();
    if(checked){
        scene->setMode(2);
        scene->setRGBA1(colors[COLORS::LINE_COLOR].red(),colors[COLORS::LINE_COLOR].green(),colors[COLORS::LINE_COLOR].blue(),colors[COLORS::LINE_COLOR].alpha());
    }
}

void MainWindow::on_fill_checkbox_toggled(bool checked)
{
    ui->color_triangle_fill->setEnabled(checked);
    scene->setFill(checked);
    if(checked) scene->setRGBA2(colors[COLORS::FILL_COLOR].red(),colors[COLORS::FILL_COLOR].green(),colors[COLORS::FILL_COLOR].blue(),colors[COLORS::FILL_COLOR].alpha());
}

void MainWindow::changeColor(COLORS number){
    if(number>=COLORS::COLORS_COUNT) return;
    QColorDialog::ColorDialogOptions options({QColorDialog::ShowAlphaChannel,QColorDialog::DontUseNativeDialog});
    QColorDialog qd(this);
    qd.setOptions(options);
    qd.setCurrentColor(colors[number]);
    if(qd.exec())
        colors[number]=qd.selectedColor();
    setSceneColor(number);
}

void MainWindow::setSceneColor(COLORS number){
    switch (number) {
    case COLORS::LINE_COLOR:
        scene->setRGBA1(colors[number].red(),colors[number].green(),colors[number].blue(),colors[number].alpha());
        break;
    case COLORS::TRIANGLE_COLOR:
        scene->setRGBA1(colors[number].red(),colors[number].green(),colors[number].blue(),colors[number].alpha());
        break;
    case COLORS::FILL_COLOR:
        scene->setRGBA2(colors[number].red(),colors[number].green(),colors[number].blue(),colors[number].alpha());
        break;
    }
}

void MainWindow::on_open_button_clicked()
{
    QFileDialog open_dialog(this);
    open_dialog.setAcceptMode(QFileDialog::AcceptOpen);
    open_dialog.setFileMode(QFileDialog::AnyFile);
    open_dialog.setViewMode(QFileDialog::Detail);
    open_dialog.setModal(true);
    QStringList list;
    if(open_dialog.exec())
        list=open_dialog.selectedFiles();
    bool stat=false;
    if(list.length()>0) stat=scene->openFile(list[0].toStdString());
    if(!enabled && stat){
        enableInterface();
    }
    if(stat) resizeBackground();
}

void MainWindow::on_zoom_in_button_clicked()
{
    scene->scaleImage(1.2);

    resizeBackground();
}

void MainWindow::on_zoom_out_button_clicked()
{
    scene->scaleImage(0.8);

    resizeBackground();
}

void MainWindow::on_hand_radiobutton_toggled(bool checked)
{
    if(checked) scene->setMode(1);
}


void MainWindow::on_color_line_clicked()
{
    changeColor(COLORS::LINE_COLOR);
}

void MainWindow::on_color_triangle_clicked()
{
    changeColor(COLORS::TRIANGLE_COLOR);
}

void MainWindow::on_color_triangle_fill_clicked()
{
    changeColor(COLORS::FILL_COLOR);
}

void MainWindow::on_color_find_clicked()
{
    changeColor(COLORS::FIND_COLOR);
}

void MainWindow::on_color_replace_clicked()
{
    changeColor(COLORS::REPLACE_COLOR);
}


void MainWindow::on_save_button_clicked()
{
    QFileDialog save_dialog(this);
    save_dialog.setAcceptMode(QFileDialog::AcceptSave);
    save_dialog.setFileMode(QFileDialog::AnyFile);
    save_dialog.setViewMode(QFileDialog::Detail);
    save_dialog.setModal(true);
    save_dialog.setNameFilter(tr("Images (*.png)"));
    QStringList list;
    if(save_dialog.exec())
        list=save_dialog.selectedFiles();
    if(list.length()>0) scene->saveFile(list[0].toStdString());
}

void MainWindow::on_find_and_replace_button_clicked()
{
    scene->findReplace(colors[COLORS::FIND_COLOR],colors[COLORS::REPLACE_COLOR]);
}

void MainWindow::on_collage_button_clicked()
{
    SizeDialog* sd=new SizeDialog(0,this);
    bool stat=false;
    if(sd->exec() && !sd->getFileName().isEmpty())
        stat=scene->collage(sd->getFileName(),sd->getWidth(),sd->getHeight());
    delete sd;
    if(!enabled && stat){
       enableInterface();
    }
    if(stat) resizeBackground();
}

void MainWindow::on_size_edit_valueChanged(int arg1)
{
    scene->setSize(arg1);
}

void MainWindow::on_create_button_clicked()
{
    SizeDialog* sd=new SizeDialog(1,this);
    bool stat=false;
    if(sd->exec())
        stat=scene->createImage(sd->getWidth(),sd->getHeight());
    delete sd;
    if(!enabled && stat){
        enableInterface();
    }
    if(stat) resizeBackground();
}

void MainWindow::menuHelp(){
    HelpDialog hd(this);
    hd.exec();
}

void MainWindow::resizeBackground(){
    QRect new_geom,geom=ui->graphicsView->geometry();
    new_geom.setLeft(geom.left()+ui->centralwidget->geometry().left()+1);
    new_geom.setTop(geom.top()+ui->centralwidget->geometry().top()+1);
    if(geom.height()>=MAX_HEIGHT || geom.width()>=MAX_WIDTH){
        new_geom.setWidth(geom.width()-(SLIDEBAR_SIZE+BORDER_SIZE));
        new_geom.setHeight(geom.height()-(SLIDEBAR_SIZE+BORDER_SIZE));
    }else{
        new_geom.setHeight(geom.height()-BORDER_SIZE);
        new_geom.setWidth(geom.width()-BORDER_SIZE);
    }
    background->setGeometry(new_geom);
}

void MainWindow::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    scene->updateImage();
    resizeBackground();
}

void MainWindow::enableInterface(){
    enabled=true;
    ui->zoom_in_button->setEnabled(true);
    ui->zoom_out_button->setEnabled(true);
    ui->save_button->setEnabled(true);
    ui->line_radiobutton->setEnabled(true);
    ui->triangle_radiobutton->setEnabled(true);
    ui->hand_radiobutton->setEnabled(true);
    ui->size_edit->setEnabled(true);
    ui->color_find->setEnabled(true);
    ui->color_replace->setEnabled(true);
    ui->find_and_replace_button->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    background->show();
}
