#include <stdio.h>
#include <stdlib.h>
#include "sstr.h"
#include "fileio.h"


FILE* filePointer;

void initiateFilePointer() {
    filePointer = fopen("write.txt", "w+");

    if (filePointer == NULL) {
        printf("Write.txt doesn't exist -- cannot show output");
        exit(1);
    }
}

void writeToFile(sstr* strObj) {
        if (filePointer == NULL)
            initiateFilePointer();
        
        int stringLength = (*strObj).currentLength;
        int property_Index = (*strObj).property_num;
        int verifiedSegments = (*strObj).global_verified_seg;
        int unverifiedSegments = (*strObj).global_checked_seg - verifiedSegments;
        
        fwrite((*strObj).charArray , stringLength, 1 , filePointer);
        fwrite("\n", 1, 1, filePointer);
        fprintf(filePointer, "%d", verifiedSegments);

        printf("\nFinal Concatenated String: ");
        for(int index = 0; index < stringLength; index++)
            printf("%c", (*strObj).charArray[index]);
        printf("\nTest case for property %d.\nNumber of verified segments: %d. \nNumber of unverified segments: %d. \n",property_Index, verifiedSegments, unverifiedSegments);


        fclose(filePointer);
}


