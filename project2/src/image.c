#include "image.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

/**
 * Initialize the image @p `img` with width @p `w` and height @p `h`.
 */
struct image* image_init(int const w, int const h) {
    // DO NOT EDIT
    struct image* img = malloc(sizeof(struct image));
    img->w = w;
    img->h = h;
    img->pixels = malloc(w * h * sizeof(*img->pixels));
    memset(img->pixels, 0, w * h * sizeof(*img->pixels));
    return img;
}

/**
 * Destroy the image @p `img` by freeing its pixels field and by freeing @p
 * `img` itself. Don't use @p img afterwards.
 */
void image_destroy(struct image* img) {
    free(img->pixels);
    free(img);
}

/**
 * Read an image from the file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 * @returns the image that was read.
 */
struct image* image_read_from_file(const char* filename) {
    // DO NOT EDIT
    FILE* f = fopen(filename, "r");
    if (f == NULL) exit(EXIT_FAILURE);

    if (fscanf(f, "P3") == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    int w, h;
    if (fscanf(f, "%d %d 255 ", &w, &h) == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }
    if (w <= 0 || h <= 0) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    struct image* img = image_init(w, h);
    struct pixel* pixels = img->pixels;

    for (int y = 0; y < img->h; ++y) {
        unsigned int r, g, b;
        for (int x = 0; x < img->w; ++x, ++pixels) {
            if (fscanf(f, "%u %u %u ", &r, &g, &b) == EOF) {
                image_destroy(img);
                fclose(f);
                exit(EXIT_FAILURE);
            }
            pixels->r = r;
            pixels->g = g;
            pixels->b = b;
        }
    }

    if (fgetc(f) != EOF) {
        image_destroy(img);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fclose(f);
    return img;
}

/**
 * Write the image @p `img` to file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 */
void image_write_to_file(struct image* img, const char* filename) {
    // TODO implement (assignment 3.3)
    int w =img->w;
    int h=img->h;
     FILE*open= fopen(filename,"w");
     fprintf(open,"P3\n");
     fprintf(open,"%d %d\n",w,h);
     fprintf(open,"255\n");
     for(int j=0;j<h;j++){
         for(int i=0;i<w;i++){

        fprintf(open,"%d %d %d\t", img->pixels[yx_index(j,i,w)].r,img->pixels[yx_index(j,i,w)].g,img->pixels[yx_index(j,i,w)].b);
         }
         fprintf(open,"\n");
     }
     fclose(open);
}






/**
 * Compute the brightness of the image @p `img`.
 */
uint8_t image_brightness(struct image* img) {
    // TODO implement (assignment 3.1)
    int sum = 0;
    int area = img->w * img->h;

    for(int i =0;i<area; ++i){
         int pixel_brightness=(img->pixels[i].r+img->pixels[i].b+img->pixels[i].g)/3;
         sum+=pixel_brightness;
    }
    int brightness = sum/area;
    return(brightness);
}

/**
 * Carve out the path @p `seam` from the image @p `img`,
 * where only the @p `w` left columns are considered.
 * Move all pixels right of it one to the left and fill the rightmost row with
 * black (0,0,0). Columns with index >= `w` are not considered as part of the
 * image.
 */
void carve_path(struct image* const img, int const w,
                uint32_t const* const seam) {
    // TODO implement (assignment 3.3)
    int i,j;
    int h=img->h;
    int w1= img->w;

    for(j=0;j<h;j++){
        for(i=seam[j];i<w-1;i++){
           img->pixels[yx_index(j,i,w1)]=img->pixels[yx_index(j,i+1,w1)];


        }

    }
    for(int j=0;j<h;j++){
       img->pixels[yx_index(j,w-1,w1)].r=0;
       img->pixels[yx_index(j,w-1,w1)].g=0;
       img->pixels[yx_index(j,w-1,w1)].b=0;

    }
}



