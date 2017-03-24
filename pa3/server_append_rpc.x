/*
 * server_append_rpc.x  Specification of the remote append server
 */
  

struct AppendArguments {
    int property_Index;
    int substring_Length;
    int substring_Partitions;
    string verify_host_name<>;
};

program SERVERAPPENDPROG {
    version SERVERAPPENDVERS {
        int RPCINITAPPENDSERVER(AppendArguments) = 1;    /* procedure number = 1 */
        int RPCAPPEND(char) = 2;                      /* procedure number = 2 */
    } = 1;                          /* version number = 1 */
} = 0x22934553;                     /* program number = 0x22934555 */

