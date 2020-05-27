#ifndef MYPNGHPP
#define MYPNGHPP
#include<png.h>
#include"exception.hpp"
#include<string>

/* Struct for RGBA colors
 * If image have color type RGB, 
 * last field not used
 */

struct RGBA{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

/* Struct Point contains the coordinates of the point
 */

struct Point{
    int x;
    int y;
};


/* Class PNG for storing PNG images
 * Image processing functions
 * work only with RGB and RGBA
 */

class PNG{
private:
    unsigned int width,height; //sizes of image

    int pixel_bytes;    //bytes per pixel

    int color_type;            //Color type of image

    int bit_depth;             //Depth of bit

    png_structp png_ptr;            //Pointer of struct png from png.h

    png_infop info_ptr;             //Pointer of png info from png.h

    int number_of_passes;           //Number of passes

    int interlace_type,compression_type,filter_type;

    png_byte* image;    //Array where the image is stored

    /* This function for findAndReplace
     * It find max rectangle of color rgba
     * from point p. p1 is a pointer where
     * stored point of top left, 
     * p2 -- point of bottom right
     */
    void findMaxRect(Point p, Point* p1,Point* p2,unsigned long* max, RGBA rgba);
public:
    PNG(); //Constructor, initialize object

    void openPNG(const std::string& file_name); //Read PNG from file

    void writePNG(const std::string& file_name ); //Write PNG to file

    void freePNG(); //Free memory of image and info_ptr

    bool isEmpty(); //PNG is empty, if image==NULL

    ~PNG(); //Destructor, free memory

    /*  Create empty PNG image
     * If there is no memory, no memory will be allocate
     */
    void createEmpty(unsigned long int width, unsigned long int height, png_byte depth=8, png_byte color_type=PNG_COLOR_TYPE_RGBA);

    /* Draws line from (x1,y1) to (x2,y2)
     * rgba -- line color
     * size -- line size
     * Last parameter for function paintTriangle()
     */
    void line(Point p1,Point p2, RGBA rgba, unsigned int size, Point* for_triangle=NULL);

    /* Create Collage from png image
     * num_n -- count on OX
     * num_m -- count on OY
     */

    void createCollage(const std::string file_name,unsigned int num_n,unsigned int num_m);

    /* Draws a triangle whose vertices are p1, p2, p3
     * rgba -- ribs color
     * Size define thickness of ribs
     */

    void drawTriangle(Point p1, Point p2, Point p3, RGBA rgba, unsigned int size);

    /* Same as drowTriangle, but fill it by color fill_rgba
     */

    void drawTriangle(Point p1, Point p2, Point p3, RGBA rgba, unsigned int size,RGBA fill_rgba);

    /* Finds the largest rectangle of color find
     * and replaces it with color replace
     */

    void findAndReplace(RGBA find, RGBA replace);

    /* Return width
    */

    unsigned int getWidth();

    /*  Return Height
    */

    unsigned int getHeight();

    /*  Return image
    */

    const unsigned char* getData();

    /*  Return color_type
    */

    int getColor();
};

#endif //End of MYPNGHPP