#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    ui->textBrowser->setText(tr("The program works only with PNG format and only with color types such as RGB and RGBA!\n\n\
\"Create\" button: Creates an empty image with the entered dimensions \"height\" and \"width\"\n\n\
\"Open\" Button: Opens a PNG file\n\n\
\"Save\" Button: Saves the image.\n\n\
\"+\" Button: Enlarges the image. Be careful, with a strong increase and requires a lot of memory !!!\n\n\
\"-\" Button: Reduces the image\n\n\
Button \"Collage\": A collage is created from the picture with the entered dimensions N * M\n\n\
Functional:\n\
1) Line drawing\n\
2) Drawing a triangle, can be filled\n\
3) Replace the largest rectangle of the given color(without alpha channel) with the selected. It may take a long time for a large image\n\n\
For drawing you need to click on the places in the image\n\n\
By default, all colors are white\n\
To change the color you need to click the \"Select color\" button under the desired action"));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
