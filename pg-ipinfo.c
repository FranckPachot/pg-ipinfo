#include "postgres.h"
#include "fmgr.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define HTTP_HOST "ipinfo.io"
#define HTTP_PATH "/"
#define MAX_RESPONSE_SIZE 1024


// Function to send an HTTP GET request using raw sockets
int send_request(const char *hostname, const char *path, char *response) {

    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("Can't open socket")));
        return -1;
    }

    server = gethostbyname(hostname);
    if (server == NULL) {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("Host not found")));
        return -1;
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(80);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("Socket: Can't connect")));
        return -1;
    }

    char request[256];
    snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, hostname);

    if (write(sockfd, request, strlen(request)) < 0) {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("Socket: Write error")));
        return -1;
    }

    int total_read = 0;
    ssize_t n;
    while ((n = read(sockfd, response + total_read, MAX_RESPONSE_SIZE - total_read)) > 0) {
        total_read += n;
    }

    response[total_read] = '\0';
    close(sockfd);

    return total_read;
}

// Function to extract text content from an HTTP response
void extract_text_content(const char *response, char *text_content) {
    char *start = strstr(response, "\r\n\r\n");
    if (start) {
        start += 4; // Move past the double CRLF
        strcpy(text_content, start);
    } else {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("No response")));
    }
}



#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1( ipinfo );

Datum ipinfo( PG_FUNCTION_ARGS )
{
    char *hostname = HTTP_HOST ;
    char *path = HTTP_PATH ;

    char response[MAX_RESPONSE_SIZE];
    char text_content[MAX_RESPONSE_SIZE] = "";
	  size_t length = 0;
	  text *t = NULL;

    if (send_request(hostname, path, response) >= 0) {
        extract_text_content(response, text_content);
        length = strlen( text_content );
        t = (text *) palloc(VARHDRSZ + length );
        SET_VARSIZE( t, VARHDRSZ + length );
        memcpy( VARDATA(t), text_content, length );
        PG_RETURN_TEXT_P( t );
    } else {
        ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg("ERROR")));
		    PG_RETURN_NULL();
    }

}
