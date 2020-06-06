#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QImage>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include "mypng.hpp"
#define MAX_MODE 3
#define MAX_WIDTH 800
#define MAX_HEIGHT 600
#define BORDER_SIZE 2

class MyScene: public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QGraphicsView* gw, QObject* parent=nullptr);

    ~MyScene();

    bool openFile(std::string filename);

    void saveFile(std::string filename);

    void scaleImage(double factor);

    void setRGBA1(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

    void setRGBA2(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

    void resetPoints();

    void setSize(unsigned int size);

    void setMode(int mode);

    void setFill(bool fill);

    void findReplace(QColor color_find,QColor color_replace);

    bool collage(QString filename, unsigned int n, unsigned int m);

    bool createImage(unsigned int width, unsigned int height);

    void updateImage();

private slots:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    // Convert error id to message
    QString getMessageFromError(PNG_ERRORS error);

    /* Draw line or triangle
     * It is defined by mode
     */
    void draw();

    QImage::Format getImageColor();
    /*  mode:
        1 -- hand
        2 -- line
        3 -- triangle
        p_count is a count of points
        size -- size of the line or edges of the triangle
    */
    int mode, p_count;
    unsigned int size;
    //Png image
    PNG* mypng;
    //Massive for points
    Point points[3];
    //Colors first and second
    RGBA rgba1, rgba2;
    //fill?
    bool fill;
    //scale
    double scale;
    //Pointer to QGraphicsView for change his dimensions
    QGraphicsView* graphicsView;
};

#endif // MYSCENE_H
