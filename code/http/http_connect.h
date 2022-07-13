#ifndef HTTP_CONNECT_H
#define HTTP_CONNECT_H

#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

#include "../log/log.h"
#include "../sqlconnpool/sql_conn_pool.h"
#include "../buffer/buffer.h"
#include "http_request.h"
#include "http_response.h"

class Http_Connect
{
public:
    Http_Connect();
    ~Http_Connect() { closeConnect(); }

    void init(int sockfd, const sockaddr_in &addr);

    ssize_t read(int *saveErrno);

    ssize_t write(int *saveErrno);

    void closeConnect();

    //获取相应的fd, addr, port, IP
    int getfd() const { return fd; }
    sockaddr_in getAddr() const { return addr; }
    int getPort() const { return addr.sin_port; }
    const char *getIP() const { return inet_ntoa(addr.sin_addr); }

    bool process();

    int toWriteBytes()
    {
        return iov[0].iov_len + iov[1].iov_len;
    }

    bool isKeepAlive() const
    {
        return request.isKeepAlive();
    }

    static bool isET;
    static const char *srcDir;
    static atomic<int> userCnt;

private:
    int fd;
    struct sockaddr_in addr;

    bool isClose;

    /*
    struct iovec {
           void  *iov_base;     //Starting address
           size_t iov_len;      //Number of bytes to transfer
       };
    */
    int iovCnt;
    struct iovec iov[2];

    Buffer readBuffer;
    Buffer writeBuffer;

    HttpRequest request;
    HttpResponse response;
};

#endif