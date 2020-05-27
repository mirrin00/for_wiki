/*
Basic functions for class PNG
- PNG()
- ~PNG()
- freePNG()
- openPNG(const std::string& file_name)
- writePNG(const std::string& file_name)
- isEmpty()
- getHeaders()
- setHeaders()
*/
#include"mypng.hpp"
#include<cstdlib>

PNG::PNG(){
    width=-1;
    height=-1;
    color_type=-1;
    bit_depth=-1;
    number_of_passes=-1;
    info_ptr=NULL;
    png_ptr=NULL;
    pixel_bytes=0;
    image=NULL;
}

void PNG::openPNG(const std::string& file_name){
    if(!isEmpty()) freePNG();
    
    FILE* file=fopen(file_name.c_str(),"rb");
    if(!file){
            throw PNGException(PNG_OPEN_CREATE_FILE_ERROR);
    }

    char header[8];

    fread(header,1,8,file);
    if(png_sig_cmp((unsigned char*)header,0,8)){
        fclose(file);
        throw PNGException(PNG_NOT_PNG);
    }

    png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

    if(!png_ptr){
        fclose(file);
        throw PNGException(PNG_READ_STRUCT_ERROR);
    }

    info_ptr=png_create_info_struct(png_ptr);

    if(!info_ptr){
        png_destroy_read_struct(&png_ptr,NULL,NULL);
        fclose(file);
        throw PNGException(PNG_INFO_STRUCT_ERROR);
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_INITIALIZATION_ERROR);
    }

    png_init_io(png_ptr,file);
    png_set_sig_bytes(png_ptr,8);
    png_read_info(png_ptr,info_ptr);

    png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
                        &interlace_type,&compression_type,&filter_type);

    if(color_type!=PNG_COLOR_TYPE_RGBA && color_type!=PNG_COLOR_TYPE_RGB){
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_COLOR_ERROR);
    }
    
    number_of_passes=png_set_interlace_handling(png_ptr);

    if(color_type==PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if(color_type==PNG_COLOR_TYPE_GRAY && bit_depth<8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    
    png_read_update_info(png_ptr,info_ptr);

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_READ_IMAGE_ERROR);
    }

    pixel_bytes=png_get_rowbytes(png_ptr,info_ptr)/width;

    image=(png_byte*)malloc(sizeof(png_byte)*height*width*pixel_bytes);
    if(!image){
        png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_MEMORY_ERROR);
    }
    for(int pass=0;pass<number_of_passes;pass++){
        for(int i=0;i<height;i++){
            png_read_row(png_ptr,image+pixel_bytes*i*width,NULL);
        }
    }
    png_read_end(png_ptr,info_ptr);
    
    png_destroy_read_struct(&png_ptr,NULL,NULL);
    png_ptr=NULL;

    fclose(file);

}

void PNG::writePNG(const std::string& file_name){
    if(isEmpty()) throw PNGException(PNG_EMPTY_ERROR);
    FILE* file=fopen(file_name.c_str(),"wb");
    if(!file){
            throw PNGException(PNG_OPEN_CREATE_FILE_ERROR);
    }
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if(!png_ptr){
        fclose(file);
        throw PNGException(PNG_WRITE_STRUCT_ERROR);
    }

    if(!info_ptr) info_ptr=png_create_info_struct(png_ptr);

    if(!info_ptr){
        png_destroy_write_struct(&png_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_INFO_STRUCT_ERROR);
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_write_struct(&png_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_INITIALIZATION_ERROR);
    }

    png_init_io(png_ptr,file);

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_write_struct(&png_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_WRITE_INFO_ERROR);
    }

    png_set_IHDR(png_ptr,info_ptr,width,height,bit_depth,color_type,
                    interlace_type,compression_type,
                    filter_type);
    

    png_write_info(png_ptr, info_ptr);

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_write_struct(&png_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_WRITE_IMAGE_ERROR);
    }
    for(int pass=0;pass<number_of_passes;pass++){
        for(int i=0;i<height;i++){
            png_write_row(png_ptr,image+pixel_bytes*i*width);
        }
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_write_struct(&png_ptr,NULL);
        fclose(file);
        throw PNGException(PNG_WRITE_END_ERROR);
    }
    png_write_end(png_ptr, NULL);

    fclose(file);

    png_destroy_write_struct(&png_ptr,NULL);
    png_ptr=NULL;
    
}


void PNG::freePNG(){
    png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    png_destroy_info_struct(png_ptr,&info_ptr);
    png_destroy_write_struct(&png_ptr,NULL);
    png_ptr=NULL;
    info_ptr=NULL;
    
    if(!image)
        return;
    free(image);
    image=NULL;
}

PNG::~PNG(){
        freePNG();
}

bool PNG::isEmpty(){
    if(!image) return true;
    return false;
}

unsigned int PNG::getWidth(){ return width; }

unsigned int PNG::getHeight(){ return height; }

const unsigned char* PNG::getData(){ return (unsigned char*)image; }

int PNG::getColor(){ return color_type; }
