#include "ppm.h"

// MPI Variables
int numOfCommunicators;
int communicator_rank;

// CLI Input Arguments
int blurRadius;
char *input_ppm_filename;
char *output_ppm_filename;

// Input PPM Parameters
Image *inputPPM;
int input_ppm_height;
int input_ppm_width;



void extractCLIArgs(char* argv[]);
struct Pixel **populateMatrix(Image* matrix, int minRow, int maxRows, int numCols);
int mpi_pixel_init(MPI_Datatype *mpi_pixel);