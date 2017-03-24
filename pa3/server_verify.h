typedef struct sstr {
    int charIndex;
    char* charArray;
} sstr;


int thread_count;
int substring_Length;
int substring_Partitions;

void initServerAddresses();
void createVerifyServerSocket();
void acceptConnection();
void receiveString();
void *socketWork();


