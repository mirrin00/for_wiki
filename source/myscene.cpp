#include "myscene.h"

MyScene::MyScene(QGraphicsView* gw, QObject* parent): QGraphicsScene(parent){
    mode=1;
    mypng=new PNG();
    size=p_count=0;
    rgba1=rgba2={255,255,255,255};
    scale=1.0;
    graphicsView=gw;
    fill=false;

}

MyScene::~MyScene(){
    mypng->freePNG();
    delete mypng;
}

void MyScene::updateImage(){
    if(mypng->isEmpty()) return;
    QImage* new_image=new QImage( QImage(mypng->getData(),mypng->getWidth(),mypng->getHeight(),getImageColor()).scaled(mypng->getWidth()*scale,mypng->getHeight()*scale));
    clear();
    addPixmap(QPixmap::fromImage(*new_image));
    setSceneRect(0,0,new_image->width(),new_image->height());
    int new_width=width()+2,new_height=height()+2;
    if(new_width>=MAX_WIDTH) new_width=MAX_WIDTH;
    if(new_height>=MAX_HEIGHT) new_height=MAX_HEIGHT;
    QRect rect=graphicsView->geometry();
    graphicsView->setGeometry(rect.left(),rect.top(),new_width,new_height);
    delete new_image;
}

bool MyScene::openFile(std::string filename){
    try {
        mypng->openPNG(filename);
        scale=1.0;
        updateImage();
    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
        return false;
    }
    return true;
}

void MyScene::saveFile(std::string filename){
    try{
        mypng->writePNG(filename);

    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
    }
}

void MyScene::scaleImage(double factor){
    if(mypng->isEmpty()) return;
    scale*=factor;
    updateImage();
}

void MyScene::setRGBA1(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
    rgba1={red,green,blue,alpha};
}

void MyScene::setRGBA2(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
    rgba2={red,green,blue,alpha};
}

void MyScene::resetPoints(){
    p_count=0;
}

void MyScene::setSize(unsigned int size){
    this->size=size;
}

void MyScene::setMode(int mode){
    if(mode>MAX_MODE) return;
    this->mode=mode;
}

void MyScene::setFill(bool fill){
    this->fill=fill;
}

void MyScene::findReplace(QColor color_find, QColor color_replace){
    try{
        RGBA c1={(unsigned char)color_find.red(),(unsigned char)color_find.green(),(unsigned char)color_find.blue(),(unsigned char)color_find.alpha()};
        RGBA c2={(unsigned char)color_replace.red(),(unsigned char)color_replace.green(),(unsigned char)color_replace.blue(),(unsigned char)color_replace.alpha()};
        mypng->findAndReplace(c1,c2);
        updateImage();
    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
    }
}

void MyScene::draw(){
    if(p_count!=mode) return;
    try{
        switch (mode) {
        case 2:
            mypng->line(points[0],points[1],rgba1,size);
            break;
        case 3:
            if(fill) mypng->drawTriangle(points[0],points[1],points[2],rgba1,size,rgba2);
            else mypng->drawTriangle(points[0],points[1],points[2],rgba1,size);
            break;
        }
    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
    }
    p_count=0;
    updateImage();
}

QImage::Format MyScene::getImageColor(){
    switch (mypng->getColor()) {
    case PNG_COLOR_TYPE_RGB:
        return QImage::Format_RGB888;
    case PNG_COLOR_TYPE_RGBA:
        return QImage::Format_RGBA8888;
    }
    return QImage::Format_Invalid;
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(mode<2) return;
    if(event->button()==Qt::MouseButton::RightButton){
        p_count=0;
        return;
    }
    QPoint qp=event->scenePos().toPoint();
    int x=qp.x()/scale,y=qp.y()/scale;
    if(x>=mypng->getWidth()) x=mypng->getWidth()-1;
    if(y>=mypng->getHeight()) y=mypng->getHeight()-1;
    points[p_count++]={x,y};
    draw();
}

bool MyScene::collage(QString filename, unsigned int n, unsigned int m){
    try{
        mypng->createCollage(filename.toStdString(),n,m);
        scale=1.0;
        updateImage();
    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
        return false;
    }
    return true;
}

bool MyScene::createImage(unsigned int width, unsigned int height){
    try{
        mypng->createEmpty(width,height);
        scale=1.0;
        updateImage();
    } catch (PNGException& e) {
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about error"));
        msgbox.setText(getMessageFromError(e.getId()));
        msgbox.exec();
        return false;
    }
    return true;
}



QString MyScene::getMessageFromError(int error){
    switch (error) {
    case PNG_OPEN_CREATE_FILE_ERROR:
        return tr("Could not open/create file");
    case PNG_NOT_PNG:
        return tr("File is no PNG");
    case PNG_READ_STRUCT_ERROR:
        return tr("Could not create struct for reading PNG");
    case PNG_INFO_STRUCT_ERROR:
        return tr("Could not create struct for information about PNG");
    case PNG_INITIALIZATION_ERROR:
        return tr("Error while reading information");
    case PNG_READ_IMAGE_ERROR:
        return tr("Error while reading image");
    case PNG_MEMORY_ERROR:
        return tr("Out of memory for this action");
    case PNG_EMPTY_ERROR:
        return tr("No open file");
    case PNG_WRITE_STRUCT_ERROR:
        return tr("Could not create struct for writing PNG");
    case PNG_WRITE_INFO_ERROR:
        return tr("Could not create struct for writing information about PNG");
    case PNG_WRITE_IMAGE_ERROR:
        return tr("Error while writing image");
    case PNG_WRITE_END_ERROR:
        return tr("Error while writing end of file");
    case PNG_COLOR_ERROR:
        return tr("PNG have a unsupported color type");
    case PNG_COORDINATES_ERROR:
        return tr("Incorrect coordinates");
    case PNG_INVALID_COUNTS:
        return tr("Inccorect size");
    }
    return tr("Unknown error");
}


