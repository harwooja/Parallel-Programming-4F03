/*
 * server_verify_rpc.x  Specification of the remote verify server
 */
  
struct VerifyArguments {
    int thread_count;
    int substring_Length;
    int substring_Partitions;
};

program SERVERVERIFYPROG {
    version SERVERVERIFYVERS {
        int RPCINITVERIFYSERVER(VerifyArguments) = 1;    /* procedure number = 1 */
        int RPCGETSEG() = 2;
    } = 1;                          /* version number = 1 */
} = 0x24934553;                     /* program number = 0x24934555 */

