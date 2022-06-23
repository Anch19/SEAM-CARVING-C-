#include "energy.h"

#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

uint32_t min(uint32_t a, uint32_t b) { return a < b ? a : b; }

/**
 * Calculate the difference of two color values @p a and @p b.
 * The result is the sum of the squares of the differences of the three (red,
 * green and blue) color components.
 * */
inline uint32_t diff_color(struct pixel const a, struct pixel const b) {
    // TODO implement (assignment 3.2)
    int red=(a.r-b.r)*(a.r-b.r);
    int blue=(a.b-b.b)*(a.b-b.b);
    int difference=red+(a.g-b.g)*(a.g-b.g)+blue;
    return difference;
}

/**
 * Calculate the total energy at every pixel of the image @p `img`,
 * but only considering columns with index less than @p `w`.
 * To this end, first calculate the local energy and use it to calculate the
 * total energy.
 * @p `energy` is expected to have allocated enough space
 * to represent the energy for every pixel of the whole image @p `img.
 * @p `w` is the width up to (excluding) which column in the image the energy
 * should be calculated. The energy is expected to be stored exactly analogous
 * to the image, i.e. you should be able to access the energy of a pixel with
 * the same array index.
 */
void calculate_energy(uint32_t* const energy, struct image* const img,int const w) {
    // TODO implement (assignment 3.2)

 int i;
    int j;



        for(j=0; j<img->h;j++){
        for(i=0; i<img->w ;i++){

            if(i==0&&j==0){
                energy [ yx_index(j,i,w)]= 0;
            }
            else if(i !=0&&j==0){
                    energy[yx_index(j,i,w)]=diff_color(img->pixels[yx_index(j,i,w)],img->pixels[yx_index(j,i-1,w)]);
                }
            else if(i==0&&j!=0){
                    energy[yx_index(j,i,w)]=diff_color(img->pixels[yx_index(j,i,w)],img->pixels[yx_index(j-1,i,w)]);
                }
                //now we will do it for the  ase where both i and j are not zero
            else{
                    energy[yx_index(j,i,w)]=diff_color(img->pixels[yx_index(j,i,w)],img->pixels[yx_index(j,i-1,w)])+diff_color(img->pixels[yx_index(j,i,w)],img->pixels[yx_index(j-1,i,w)]);
                }
        }

    }
    for(j=0; j<img->h;j++){
    for(i=0; i<img->w ;i++){



            if(i==0&&j==0){
                energy [ yx_index(j,i,w)]= 0; //top left pixel
            }
            //top row
            else if(i!=0&&j==0){
                energy[yx_index(j,i,w)]=energy[yx_index(j,i,w)];
            }//
            //left coloumn
            else if(j!=0&&i==0){
                energy[yx_index(j,i,w)]+=min(energy[yx_index(j-1,i+1,w)],energy[yx_index(j-1,i,w)]);
            }//right coloumn
            else if(i==w-1){
                energy[yx_index(j,i,w)]+=min(energy[yx_index(j-1,i,w)],energy[yx_index(j-1,i-1,w)]);
            }
            else{
                 energy[yx_index(j,i,w)]+=min(min(energy[yx_index(j-1,i-1,w)],energy[yx_index(j-1,i,w)]),energy[yx_index(j-1,i+1,w)]);
             }
        }
    }










}

/**
 * Calculate the index of the column with the least energy in bottom row.
 * Expects that @p `energy` holds the energy of every pixel of @p `img` up to
 * column (excluding) @p `w`. Columns with index `>= w` are not considered as
 * part of the image.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
int calculate_min_energy_column(uint32_t const* const energy, int const w0,
                                int const w, int const h) {
    // TODO implement (assignment 3.2)

    int i=0;
    int j=0;

    int save=0;

    for(j=h-1;j<h;j++){

        uint32_t minimum = energy[yx_index(h-1, 0, w0)];

        for(i=1;i<w;i++){
         if(minimum>energy [yx_index(h-1,i,w0)]){
             minimum=energy [yx_index(h-1,i,w0)];
         save = i;



        }
    }
    }
return save;
}
/**
 * Calculate the optimal path (i.e. least energy), according to the energy
 * entries in @p `energy` up to (excluding) column @p `w`. The path is stored in
 * @p `seam`. Columns with index `>= w` are not considered as part of the image.
 * @p `x` is the index in the bottom row where the seam starts.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
void calculate_optimal_path(uint32_t const* const energy, int const w0,
                            int const w, int const h, int x,
                            uint32_t* const seam) {
    // TODO implement (assignment 3.2)

    int j;


    seam[h-1]=x;
    for(j=h-1;j>0;j--){
         int left_pixel= energy [yx_index(j-1,x-1,w0)];
   int midlle_pixel= energy [yx_index(j-1,x,w0)];
   int right_pixel = energy [yx_index(j-1,x+1,w0)];

      if (x==0 && (midlle_pixel < right_pixel)){

        seam[j-1]=x;
        x=x;
      }
      else if(x==0 && (midlle_pixel > right_pixel)){
          seam[j-1]=x+1;
          x=x+1;

      }
      else if(x==0 && (midlle_pixel ==right_pixel)){
          seam[j-1]=x;
          x=x;

      }

      //last coloumn
      else if(x==w-1 && midlle_pixel > left_pixel){
          seam[j-1]=x-1;
          x=x-1;

      }
      else if(x==w-1 && midlle_pixel < left_pixel){
          seam[j-1]=x;
          x=x;

      }
      else if(x==w-1 && midlle_pixel == left_pixel){
          seam[j-1]=x;
          x=x;

      }
      else if (left_pixel>=midlle_pixel&&midlle_pixel<=right_pixel){
          seam[j-1]=x;
          x=x;
      }
      else if(left_pixel<=midlle_pixel&&left_pixel<=right_pixel){
          seam[j-1]=x-1;
          x=x-1;
      }
      else{
          seam[j-1]=x+1;
          x=x+1;
      }
    }
}








