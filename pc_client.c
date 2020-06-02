#include <stdio.h>
#include <stdlib.h>

#include "transfer.h"

using namespace std;

int main(int argc, char *argv[])
{
    // char buffer[256];
    // Hardcoded file params.
    char INPUT_FILE[] = "/home/tejaspan/gits/mdk/custom/MvNci_test/data/test.raw";
    int INPUT_FILE_SIZE = 18747;

    // Create buffer to read file.
    char file_buffer[INPUT_FILE_SIZE];

    // Read file.
    ifstream fin(INPUT_FILE, ios::in | ios::binary);
    fin.read(file_buffer, INPUT_FILE_SIZE);

    // Type of request.
    char type = 'b';

    printf("Buffer Size:\t\t%ld\n", sizeof(file_buffer));
    printf("Msg Type Size:\t\t%ld\n", sizeof(type));
    printf("Msg Buffer Size:\t%ld\n", sizeof(INPUT_FILE_SIZE));

    char packet[PACKETSIZE];

    serialize(type, file_buffer, INPUT_FILE_SIZE, packet);

    Transfer transfer;
    const char *hostname = "localhost";
    client_init(transfer, hostname, 8810);

    int n = write(transfer.server_sock, packet, PACKETSIZE);
    if (n < 0)
    {
        perror("ERROR Writing to socket");
    }

    close_connection(transfer);
    return 0;
}