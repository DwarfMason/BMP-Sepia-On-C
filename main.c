    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <windef.h>
    #include <math.h>

#pragma pack(push)
    #pragma pack(1)
    struct bmpHeader
    {
        unsigned char	b1,b2;			//Символы BM (смение 0, длина 2)
        unsigned int	bfSize;			//Размер файла в байтах	(Смещение 2, длина 2)
        unsigned short	bfReserved1;	        //Бесполезно (Смещение 6, длина 2)
        unsigned short	bfReserved2;	        //Бесполезно (Смещение 8, длина 2)
        unsigned int	bfOffBits;		//Смещение до самого изображения (Смещение 10, длина 2)
    };
    #pragma pack(pop);

    #pragma pack(push)
    #pragma pack(1)

    struct bmpInfo
    {
        unsigned int bisize;        //Размер данной структуры в байтах, указывающий также на версию структуры (здесь должно быть значение 12).
        unsigned int biwidth;     //Ширина (bcWidth) и высота (bcHeight) растра в пикселях. Указываются целым числом без знака. Значение 0 не документировано.
        unsigned int biheight;
        unsigned short biplanes;    //В BMP допустимо только значение 1. Это поле используется в значках и курсорах Windows.
        unsigned short bcbitcount;  //Количество бит на пиксель (список поддерживаемых смотрите в отдельном разделе ниже).
        unsigned int biCompression; //Указывает на способ хранения пикселей.
        unsigned int biSizeImage; //Размер пиксельных данных в байтах. Может быть обнулено если хранение осуществляется двумерным массивом.
        unsigned int biXPelsPerMeter;
        unsigned int biYPelsPerMeter; // Количество пикселей на метр по горизонтали и вертикали.
        unsigned int biClrUsed; //Размер таблицы цветов в ячейках.
        unsigned int biClrImportant; //Количество ячеек от начала таблицы цветов до последней используемой (включая её саму).
    };
    #pragma pack(pop);

    struct Px{
        unsigned char Red;  //Это красный цвет, как ни странно
        unsigned char Green; //Это зеленый цвет, как ни странно
        unsigned char Blue; //Это синий цвет, опять же странно
    };



    int main(int argc, char **argv) {

          if (strcmp(argv[1],"-help") == 0){
        printf("This program adds Sepia effect to your BMP image\n"
                       "Type sepia <input name> <opacity coefficient> <output name>\n"
                      "The program should be ine the same directory with the image\n"
                      "Program works with 24bait files");
        return 0;
    }
        if (strcmp(argv[1], "sepia") == 0) {
            char *input = argv[2];
            float opacity = atof(argv[3]);
            char *output = argv[4];
            FILE *in, *out;
            struct bmpHeader hdBMP;
            struct bmpInfo infBMP;
            if ((in = fopen(input, "rb")) == NULL)
                return -1;
            out = fopen(output, "wb+");
            fread(&hdBMP, sizeof(hdBMP), 1, in);
            fread(&infBMP, sizeof(infBMP), 1, in);
            struct Px pxArray[infBMP.biwidth][infBMP.biheight];
            fseek(in, hdBMP.bfOffBits, 0);
            for (int i = 0; i < infBMP.biwidth; i++) {
                for (int j = 0; j < infBMP.biheight; j++) {
                    fread(&pxArray[i][j], sizeof(pxArray[0][0]), 1, in);
                }
            }
            fclose(in);
            fwrite(&hdBMP, sizeof(hdBMP), 1, out);
            fwrite(&infBMP, sizeof(infBMP), 1, out);
            fseek(out, hdBMP.bfOffBits, 0);
            for (int i = 0; i < infBMP.biwidth; i++){
                for (int j = 0; j < infBMP.biheight; j++) {
                    pxArray[i][j].Red = pxArray[i][j].Red * (1 -opacity) + min((.393 * pxArray[i][j].Red) + (.769 * pxArray[i][j].Green) + (.189 * (pxArray[i][j].Blue)), 255.0) * opacity;
                    pxArray[i][j].Green = pxArray[i][j].Green * (1 - opacity) + min((.349 * pxArray[i][j].Red) + (.686 * pxArray[i][j].Green) + (.168 * (pxArray[i][j].Blue)), 255.0) * opacity;
                    pxArray[i][j].Blue = pxArray[i][j].Blue * (1 - opacity) + min((.272 * pxArray[i][j].Red) + (.534 * pxArray[i][j].Green) + (.131 * (pxArray[i][j].Blue)), 255.0) * opacity;
                    fwrite(&pxArray[i][j], sizeof(pxArray[0][0]), 1, out);
                }
            }
            fclose(out);
        }
        printf("Check the result!");
    return 0;
}