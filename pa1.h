void *Construct(void *rank); /*Thread function*/
float RandomBetween(float smallNumber, float bigNumber);
int *count(int start, int end, char c0, char c1, char c2);
void *verify(int property_Index, int M, int N, int L, int rank, char c0, char c1, char c2 );
void writeToFile(int property_Index, int verifiedSegments, int unverifiedSegments);

struct SOB {
    int property_num;
    int number_of_threads;
    char c0;
    char c1;
    char c2;
    int substring_Length;
    int number_of_Segments;
    int currentLength;
    int charIndex;
    char* charArray;
};