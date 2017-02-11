typedef struct sstr {
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
    int global_checked_seg;
    int global_verified_seg;
} sstr;

void initiateStruct(int substring_Length, int substring_Partitions, char c0, char c1, char c2, int thread_count, int property_Index);
void *Construct(void *rank); /*Thread function*/
int *count(int start, int end, char c0, char c1, char c2);
void *verify(int property_Index, int M, int N, int L, int rank, char c0, char c1, char c2 );
float RandomBetween(float smallNumber, float bigNumber);
sstr* returnStrObj();
