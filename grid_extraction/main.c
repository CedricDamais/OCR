#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "utils.h"
#include "grid_extract.h"
#include "row_extract.h"
#include "grid_numbers_extraction.h"
#include "../image_transformation/image/image_processing.h"





/*int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    SDL_Surface* src_surface = load_image(argv[1]);

    printf("Grid Detection: launching x coordinate detection.\n");

    int x = grid_x_coordinate(src_surface);

    printf("Grid Detection: phase 1 done!: %i\n", x);

    int y_top = grid_y_top_coordinate(src_surface, x);

    int y_bottom = grid_y_bottom_coordinate(src_surface, x);

    printf("Grid Detection: phase 2 done!\n");

    int extracted = extract_grid(src_surface, x, y_top, y_bottom);
    if (extracted)
    {
        printf("Grid Detection: starting row extraction.\n");
        int row_extracted = grid_row_extract("c_image.jpeg");
        if (row_extracted)
        {
            printf("Grid Detection: extraction done.\n");
            int numbers_extracted = grid_numbers_full_extract();
            
            if (numbers_extracted)
            {
                printf("Grid Numbers extraction: done!\n");
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
*/
