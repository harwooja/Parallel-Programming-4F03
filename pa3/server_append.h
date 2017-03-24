typedef struct sstr {
    int charIndex;
    char *charArray;
} sstr;


int property_Index;
int substring_Length;
int substring_Partitions;
char *verify_host_name;


void initServerAddresses();
void constructString(char c);
void connectVerifyServer();
void sendString();
