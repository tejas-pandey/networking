#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "transfer.h"

using namespace std;

void callback(int i, Transfer &transfer)
{
    char OUTPUT_FILE[] = "/home/tejaspan/gits/mdk/custom/MvNci_test/data/test_blah2.raw";
    // int INPUT_FILE_SIZE = 18747;

    // Create buffer to read file.
    char packet[PACKETSIZE];

    int n = read(transfer.client_sock, packet, PACKETSIZE);
    if (n < 0)
    {
        perror("Error reading from socket!\n");
    }

    printf("REQ ID:\t%d\n", i);

    int buffer_size = deserialize_msg(packet);
    printf("Buffer Size:\t\t%d\n", buffer_size);
    printf("Msg Type Size:\t\t%ld\n", sizeof(char));
    printf("Msg Buffer Size:\t%ld\n", sizeof(buffer_size));

    char blob[buffer_size];

    deserialize_buffer(packet, blob, buffer_size);

    ofstream fout(OUTPUT_FILE, ios::out | ios::binary);
    fout.write(blob, buffer_size);
}

int main(int argc, char *argv[])
{
    // Create socket
    Transfer transfer;
    server_init(transfer, 8810);
    server_listen(transfer, callback);
    close_connection(transfer);
    return 0;
}