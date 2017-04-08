#include "ppm.h"

int blurRadius;
char *input_ppm_filename;
char *output_ppm_filename;
struct Pixel populateMatrix(Image* matrix, int numRows, int numCols);