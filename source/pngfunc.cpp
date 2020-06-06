/*
Extended functions for class PNG
*/
#include"mypng.hpp"
#include<cstdlib>
#include<cmath>
#include<cstring>



void PNG::createEmpty(unsigned long int width, unsigned long int height, png_byte depth, png_byte color_type){
    if(color_type!=PNG_COLOR_TYPE_RGBA && color_type!=PNG_COLOR_TYPE_RGB) throw PNGException(PNG_COLOR_ERROR);
    if(!width || !height) throw PNGException(PNG_INVALID_COUNTS);
    freePNG();
    if(color_type==PNG_COLOR_TYPE_RGB) pixel_bytes=3;
    if(color_type==PNG_COLOR_TYPE_RGBA) pixel_bytes=4;
    this->color_type=color_type;
    bit_depth=depth;
    this->width=width;
    this->height=height;
    image=(png_byte*)calloc(pixel_bytes*height*width,sizeof(png_byte));
    if(!image) throw PNGException(PNG_MEMORY_ERROR);
    interlace_type=PNG_INTERLACE_NONE;
    compression_type=PNG_COMPRESSION_TYPE_DEFAULT;
    filter_type=PNG_FILTER_TYPE_DEFAULT;
    number_of_passes=1;
}

void PNG::line(Point p1,Point p2, RGBA rgba, unsigned int size, Point* for_triangle){
    if(p1.x>=width || p2.x>=width || p1.y>=height || p2.y>=height ) throw PNGException(PNG_COORDINATES_ERROR);
    if(color_type!=PNG_COLOR_TYPE_RGBA && color_type!=PNG_COLOR_TYPE_RGB) throw PNGException(PNG_COLOR_ERROR);
    if(isEmpty()) throw PNGException(PNG_EMPTY_ERROR);

    int len_x=abs(p1.x-p2.x);
    int len_y=abs(p1.y-p2.y);
    int direct_x=0;
    int direct_y=0;
    p1.x>p2.x? direct_x=-1 : direct_x=1;
    p1.y>p2.y? direct_y=-1 : direct_y=1;

    if(len_x>=len_y){
        double step_y=(double)len_x/len_y;
        
        if(len_y==0){
            step_y=0;
            direct_y=0;
        }
        int x=p1.x,y=p1.y;
        double y_count=0;
        for(int i=0;i<=len_x;i++){
            for(int j=0;j<(size+1)/2;j++){
                png_byte* pixel1,*pixel2;
                if(y+j<height) pixel1=&(image [ pixel_bytes * ((y+j) * width + x ) ]);
                else pixel1=&(image [ pixel_bytes * (y * width + x ) ]);
                if(y-j>=0) pixel2=&(image [ pixel_bytes * ((y-j) * width + x ) ]);
                else pixel2=&(image [ pixel_bytes * (y * width + x ) ]);
                pixel1[0]=pixel2[0]=rgba.red;
                pixel1[1]=pixel2[1]=rgba.green;
                pixel1[2]=pixel2[2]=rgba.blue;
                if(pixel_bytes==4) pixel1[3]=pixel2[3]=rgba.alpha;
                //for function paintTriangle()
                if(for_triangle){
                    if(width>height){
                        Point *y1,*y2;
                        if((y+j)<height) y1=for_triangle+y+j;
                        else y1=for_triangle+y;
                        if((y-j)>=0) y2=for_triangle+y-j;
                        else y2=for_triangle+y;
                        if(x<y1->x){
                            y1->x=x;
                        }
                        if(x<y2->x){
                            y2->x=x;
                        }
                        if(x>y1->y){
                            y1->y=x;
                        }
                        if(x>y2->y){
                            y2->y=x;
                        }
                    }else{
                        int y_min,y_max;
                        if((y+j)<height) y_max=y+j;
                        else y_max=y;
                        if((y-j)>=0) y_min=y-j;
                        else y_min=y;
                        if(y_max<for_triangle[x].x){
                            for_triangle[x].x=y_max;
                        }
                        if(y_min>for_triangle[x].y){
                            for_triangle[x].y=y_min;
                        }
                    }
                    
                }
            }

            x+=direct_x;
            if( (++y_count) >=step_y){
                y_count-=step_y;
                y+=direct_y;
            }
        }
    }else{
        double step_x=(double)len_y/len_x;
        
        if(len_x==0){
            step_x=0;
            direct_x=0;
        }
        int x=p1.x,y=p1.y;
        double x_count=0;
        for(int i=0;i<=len_y;i++){
            
            for(int j=0;j<(size+1)/2;j++){
                png_byte* pixel1,*pixel2;
                if(x+j<width) pixel1=&(image [ pixel_bytes * (y * width + x + j ) ]);
                else pixel1=&(image [ pixel_bytes * (y * width + x ) ]);
                if(x-j>=0) pixel2=&(image [ pixel_bytes * (y * width + x -j ) ]);
                else pixel2=&(image [ pixel_bytes * (y * width + x ) ]);
                pixel1[0]=pixel2[0]=rgba.red;
                pixel1[1]=pixel2[1]=rgba.green;
                pixel1[2]=pixel2[2]=rgba.blue;
                if(pixel_bytes==4)pixel1[3]=pixel2[3]=rgba.alpha;
                //for function paintTriangle()
                if(for_triangle){
                    if(width>height){
                        int x_min,x_max;
                        if((x+j)<width) x_max=x+j;
                        else x_max=x;
                        if((x-j)>=0) x_min=x-j;
                        else x_min=x;
                        if(x_max<for_triangle[y].x){
                            for_triangle[y].x=x_max;
                        }
                        if(x_min>for_triangle[y].y){
                            for_triangle[y].y=x_min;
                        }
                    }else{
                        Point *x1,*x2;
                        if((x+j)<width) x1=for_triangle+x+j;
                        else x1=for_triangle+x;
                        if((x-j)>=0) x2=for_triangle+x-j;
                        else x2=for_triangle+x;
                        if(y<x1->x){
                            x1->x=y;
                        }
                        if(y<x2->x){
                            x2->x=y;
                        }
                        if(y>x1->y){
                            x1->y=y;
                        }
                        if(y>x2->y){
                            x2->y=y;
                        }
                    }
                }
                
            }

            y+=direct_y;
            if( (++x_count) >=step_x){
                x_count-=step_x;
                x+=direct_x;
            }
        }
    }
}



void PNG::createCollage(const std::string file_name,unsigned int num_n,unsigned int num_m){
    if(!num_n || !num_m) throw PNGException(PNG_INVALID_COUNTS);
    if(file_name.empty()) throw PNGException(PNG_EMPTY_ERROR);
    freePNG();
    PNG png;
    png.openPNG(file_name);
    createEmpty(num_n*png.width,num_m*png.height,png.bit_depth,png.color_type);
    unsigned long int len=pixel_bytes*png.width; //len - size of one line
    for(int count=0;count<num_m;count++){
        for(int iheight=0;iheight<png.height;iheight++){
            for(int i=0;i<num_n;i++){
                memcpy( image+( len *( num_n*(count*png.height + iheight) +i ) ),
                                png.image+( iheight*len ),
                                len );
            }
        }
    }
}


void PNG::drawTriangle(Point p1, Point p2, Point p3, RGBA rgba, unsigned int size){
    if(color_type!=PNG_COLOR_TYPE_RGB && color_type!=PNG_COLOR_TYPE_RGBA) throw PNGException(PNG_COLOR_ERROR);
    if(isEmpty()) throw PNGException(PNG_EMPTY_ERROR);
    line(p1,p2,rgba,size);
    line(p2,p3,rgba,size);
    line(p3,p1,rgba,size);
}

void PNG::drawTriangle(Point p1, Point p2, Point p3, RGBA rgba, unsigned int size, RGBA fill_rgba){
    if(color_type!=PNG_COLOR_TYPE_RGB && color_type!=PNG_COLOR_TYPE_RGBA) throw PNGException(PNG_COLOR_ERROR);
    if(isEmpty()) throw PNGException(PNG_EMPTY_ERROR);
    if(!size) return;
    Point* borders;
    int min,max;
    if(width>height){
        min=p1.y;
        if(p2.y<min) min=p2.y;
        if(p3.y<min) min=p3.y;
        max=p1.y;
        if(p2.y>max) max=p2.y;
        if(p3.y>max) max=p3.y;
        borders=(Point*)malloc(sizeof(Point)*height);
    }
    else{
        min=p1.x;
        if(p2.x<min) min=p2.x;
        if(p3.x<min) min=p3.x;
        max=p1.x;
        if(p2.x>max) max=p2.x;
        if(p3.x>max) max=p3.x;
        borders=(Point*)malloc(sizeof(Point)*width);
    }
    
    if(!borders) throw PNGException(PNG_MEMORY_ERROR);

    if(width>height) for(int i=min;i<max;i++) borders[i]={width,-1};
    else for(int i=min;i<max;i++) borders[i]={height,-1};

    line(p1,p2,rgba,1,borders);
    line(p2,p3,rgba,1,borders);
    line(p3,p1,rgba,1,borders);

    if(width>height){
        for(int i=min;i<max;i++){
            for(int j=borders[i].x+1;j<borders[i].y;j++){
                png_byte* pixel=image+pixel_bytes*(i*width+j);
                pixel[0]=fill_rgba.red;
                pixel[1]=fill_rgba.green;
                pixel[2]=fill_rgba.blue;
                if(pixel_bytes==4) pixel[3]=fill_rgba.alpha;
            }
        }
    }else{
        for(int i=min;i<max;i++){
            for(int j=borders[i].x+1;j<borders[i].y;j++){
                png_byte* pixel=image+pixel_bytes*(j*width+i);
                pixel[0]=fill_rgba.red;
                pixel[1]=fill_rgba.green;
                pixel[2]=fill_rgba.blue;
                if(pixel_bytes==4) pixel[3]=fill_rgba.alpha;
            }
        }
    }
    
    drawTriangle(p1,p2,p3,rgba,size);

    free(borders);
}

void PNG::findAndReplace(RGBA find, RGBA replace){
    if(color_type!=PNG_COLOR_TYPE_RGBA && color_type!=PNG_COLOR_TYPE_RGB) throw PNGException(PNG_COLOR_ERROR);
    Point start={0,0}, end={-1,-1};
    unsigned long max=0;

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++)
            findMaxRect({j,i},&start,&end,&max,find);
    }


    for(int i=start.y;i<=end.y;i++){
        for(int j=start.x;j<=end.x;j++){
            memcpy(image+pixel_bytes*(i*width+j),&replace,3);
        }
    }

}

void PNG::findMaxRect(Point p, Point* p1,Point* p2,unsigned long* pmax, RGBA rgba){
    int max_width,max_height=1,x=p.x,y=p.y;
    unsigned long int max;
    png_byte* pixel=image+pixel_bytes*(y*width+x);
    Point end;
    while( x<width && pixel[0]==rgba.red && pixel[1]==rgba.green && pixel[2]==rgba.blue){
        x++;
        pixel+=pixel_bytes;
    }
    max_width=x-p.x;
    if(max_width){
        max=max_width;
        end.y=y;
        end.x=x;
    }
    else return;
    while(y<height){
        x=p.x;
        int now_width=0;
        bool isbreak=false;
        while(now_width<max_width){
            pixel=image+pixel_bytes*(y*width+x);
            if(pixel[0]!=rgba.red || pixel[1]!=rgba.green || pixel[2]!=rgba.blue){
                isbreak=true;
                break;
            }
            x++;
            now_width++;
        }
        if(isbreak){
            if((max_height-1)*max_width>max){
                max=(max_height-1)*max_width;
                end.y=y-1;
                end.x=p.x+max_width-1;
            }
            max_width=now_width;
        }
        y++;
        max_height++;
    }
    y--;
    max_height--;
    if(max_height*max_width>max){
                max=max_height*max_width;
                end.y=y;
                end.x=p.x+max_width-1;
            }
    if(max>*pmax){
        *pmax=max;
        *p1=p;
        *p2=end;
    }
}
