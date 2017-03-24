int property_Index;
int thread_count;
int substring_Length;
int substring_Partitions;
char c0, c1, c2;
char *append_host_name;
char *verify_host_name;
enum SERVER_TYPE {APPEND, VERIFY};
 
void runClient();
void createClientSocket();
void connectAppendServer();
void sendCharsToAppendServer();
void getSegmentFromVerifyServer();

void createRPC(char* serverHost, enum SERVER_TYPE value);
void rpcAppendServer();
void rpcVerifyServer();

float RandomBetween(float smallNumber, float bigNumber);

