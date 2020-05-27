#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QLabel>
#include <QResizeEvent>
#include "myscene.h"
#include "sizedialog.h"
#include "helpdialog.h"

#define COLORS_COUNT 5

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_triangle_radiobutton_toggled(bool checked);

    void on_line_radiobutton_toggled(bool checked);

    void on_fill_checkbox_toggled(bool checked);

    void on_hand_radiobutton_toggled(bool checked);

    void on_open_button_clicked();

    void on_zoom_in_button_clicked();

    void on_zoom_out_button_clicked();

    void on_color_line_clicked();

    void on_color_triangle_clicked();

    void on_color_triangle_fill_clicked();

    void on_color_find_clicked();

    void on_color_replace_clicked();

    void on_save_button_clicked();

    void on_find_and_replace_button_clicked();

    void on_collage_button_clicked();

    void on_size_edit_valueChanged(int arg1);

    void on_create_button_clicked();

    void menuHelp();

    void resizeEvent(QResizeEvent* event) override;

private:
    void changeColor(int number);

    void setSceneColor(int number);

    void resizeBackground();

    void enableInterface();

    Ui::MainWindow *ui;
    MyScene* scene;
    QLabel* background;
    QColor colors[COLORS_COUNT];

    bool enabled;
};
#endif // MAINWINDOW_H
