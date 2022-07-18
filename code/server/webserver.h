#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "epoller.h"
#include "../log/log.h"
#include "../sqlconnpool/sql_conn_pool.h"
#include "../threadpool/threadpool.h"
#include "../http/http_connect.h"
#include "../timer/heaptimer.h"

using namespace std;

class WebServer
{
public:
    WebServer(
        int port, int trigMode, int timeoutMs, bool optLinger,
        int sqlPort, const char *sqlUser, const char *sqlPwd,
        const char *dbName, int connPoolNum,
        int threadNum, int maxRequests,
        bool openLog, int logLevel, int logQueSize);

    ~WebServer();

    void start();

private:
    bool initSocket();
    void initEventMode(int trigMode);
    void addClient(int fd, sockaddr_in addr);

    void dealListen();
    void dealWrite(Http_Connect *client);
    void dealRead(Http_Connect *client);

    void sendError(int fd, const char *info);
    void extentTime(Http_Connect *client);
    void closeConnect(Http_Connect *client);

    void onRead(Http_Connect *client);
    void onWrite(Http_Connect *client);
    void onProcess(Http_Connect *client);

    static const int MAX_FD = 65536;
    static int setfdNonblock(int fd);

    int port;
    bool openLinger;
    int timeoutMs;
    bool shutdown;
    int listenfd;
    char *srcDir;

    uint32_t listenEvent;
    uint32_t connEvent;

    unique_ptr<HeapTimer> timer;
    unique_ptr<Epoller> epoller;
    unordered_map<int, Http_Connect> users;
};

#endif