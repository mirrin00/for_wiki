#ifndef EXCEPTION
#define EXCEPTION

#define PNG_OPEN_CREATE_FILE_ERROR 0
#define PNG_NOT_PNG 1
#define PNG_READ_STRUCT_ERROR 2
#define PNG_INFO_STRUCT_ERROR 3
#define PNG_INITIALIZATION_ERROR 4
#define PNG_READ_IMAGE_ERROR 5
#define PNG_MEMORY_ERROR 6
#define PNG_EMPTY_ERROR 7
#define PNG_WRITE_STRUCT_ERROR 8
#define PNG_WRITE_INFO_ERROR 9
#define PNG_WRITE_IMAGE_ERROR 10
#define PNG_WRITE_END_ERROR 11
#define PNG_COLOR_ERROR 12
#define PNG_COORDINATES_ERROR 13
#define PNG_INVALID_COUNTS 14


class PNGException{
private:
    int id;
public:
    PNGException(int id){
        this->id=id;
    }
    
    int getId(){return id;}
};

#endif