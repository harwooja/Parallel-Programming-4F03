void *Construct(void *rank); /*Thread function*/
float RandomBetween(float smallNumber, float bigNumber);

struct SOB {
    int substring_Length;
    int number_of_Segments;
    size_t charIndex;
    char charArray[];
};