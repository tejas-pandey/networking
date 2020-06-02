
#include "transfer.h"

void server_init(Transfer &transfer, int portno)
{
    transfer.server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (transfer.server_sock < 0)
    {
        perror("ERROR Can't open socket!\n");
        exit(10);
    }

    transfer.portno = portno;

    bzero((char *)&transfer.serv_addr, sizeof(transfer.serv_addr));

    transfer.serv_addr.sin_family = AF_INET;
    transfer.serv_addr.sin_addr.s_addr = INADDR_ANY;
    transfer.serv_addr.sin_port = htons(transfer.portno);

    if (bind(transfer.server_sock, (struct sockaddr *)&transfer.serv_addr, sizeof(transfer.serv_addr)) < 0)
    {
        perror("ERROR On binding!\n");
        exit(10);
    }
}

void client_init(Transfer &transfer, const char *hostname, int portno)
{
    transfer.server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (transfer.server_sock < 0)
    {
        perror("ERROR Can't open socket!\n");
        exit(10);
    }

    transfer.portno = portno;

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL)
    {
        perror("ERROR No such host!\n");
        exit(10);
    }

    bzero((char *)&transfer.serv_addr, sizeof(transfer.serv_addr));

    transfer.serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&transfer.serv_addr.sin_addr.s_addr,
          server->h_length);
    transfer.serv_addr.sin_port = htons(transfer.portno);

    if (connect(transfer.server_sock, (struct sockaddr *)&transfer.serv_addr, sizeof(transfer.serv_addr)) > 0)
    {
        perror("ERROR Connecting\n");
        exit(10);
    }
}

void server_listen(Transfer &transfer, void (*callback)(int, Transfer &))
{
    listen(transfer.server_sock, 5);

    int i = 0;
    while (true)
    {
        transfer.clilen = sizeof(transfer.cli_addr);
        transfer.client_sock = accept(transfer.server_sock, (struct sockaddr *)&transfer.cli_addr, &transfer.clilen);

        if (transfer.client_sock < 0)
        {
            perror("ERROR On server listen!\n");
            exit(10);
        }

        i++;
        (*callback)(i, transfer);

        close(transfer.client_sock);
    }
}

void close_connection(Transfer &transfer)
{
    close(transfer.server_sock);
}

void serialize(const char type, const char *buffer, const int buffer_size, char *packet)
{
    char *p = (char *)packet;
    *p = type;
    p++;

    int *q = (int *)p;
    *q = buffer_size;
    q++;

    char *g = (char *)q;
    int i = 0;
    while (i < buffer_size)
    {
        *g = buffer[i];
        g++;
        i++;
    }
}

int deserialize_msg(const char *packet)
{
    char *p = (char *)packet;

    if (*p == 'b')
    {
        printf("Got a blob file!\n");
        p++;
        int *q = (int *)p;
        return *q;
    }
    else
    {
        printf("Not implemented!\n");
    }
}

void deserialize_buffer(const char *packet, char *buffer, int buffer_size)
{
    char *p = (char *)packet;
    p++;
    int *q = (int *)p;
    q++;
    char *g = (char *)q;

    memcpy(buffer, g, buffer_size);
}