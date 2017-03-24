CC = gcc
CFLAGS = -std=gnu99 -Wall -fopenmp -g
LDLIBS = -lnsl
LDFLAGS = -fopenmp 
EXE = $(*.x)

all: .rpc client server_append server_verify

.rpc:
	rpcgen server_append_rpc.x 
	rpcgen server_verify_rpc.x


client: client.o server_append_rpc_clnt.o server_append_rpc_xdr.o server_verify_rpc_clnt.o server_verify_rpc_xdr.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

server_append: server_append.o server_append_rpc_svc.o server_append_rpc_xdr.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

server_verify: server_verify.o server_verify_rpc_svc.o server_verify_rpc_xdr.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm client server_append server_verify ./*.o ./*_rpc_clnt.c ./*_rpc_xdr.c ./*_rpc_svc.c ./*_rpc.h core.*


