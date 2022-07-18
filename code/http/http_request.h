#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>
#include <mysql/mysql.h>

#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../sqlconnpool/sql_conn_pool.h"
#include "../threadpool/threadpool.h"

using namespace std;
/*
       服务器处理HTTP请求的可能结果，报文解析的结果
       NO_REQUEST          :   请求不完整，需要继续读取客户数据
       GET_REQUEST         :   表示获得了一个完成的客户请求
       BAD_REQUEST         :   表示客户请求语法错误
       NO_RESOURCE         :   表示服务器没有资源
       FORBIDDEN_REQUEST   :   表示客户对资源没有足够的访问权限
       FILE_REQUEST        :   文件请求,获取文件成功
       INTERNAL_ERROR      :   表示服务器内部错误
       CLOSED_CONNECTION   :   表示客户端已经关闭连接了
   */
enum HTTP_CODE
{
    NO_REQUEST = 0,
    GET_REQUEST,
    BAD_REQUEST,
    NO_RESOURSE,
    FORBIDDENT_REQUEST,
    FILE_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
};

/*
       解析客户端请求时，主状态机的状态
       REQUEST_LINE:当前正在分析请求行
       HEADERS:当前正在分析头部字段
       BODY:当前正在解析请求体
   */
enum PARSE_STATE
{
    REQUEST_LINE = 0,
    HEADERS,
    BODY,
    FINISH
};

class HttpRequest
{
public:
    HttpRequest() { init(); }
    ~HttpRequest() = default;

    void init();
    HTTP_CODE parse(Buffer &buffer);

    string getPathConst() const { return path; }
    string &getPath() { return path; }
    string getMethod() const { return method; }
    string getVersion() const { return version; }
    bool isKeepAlive() const;

private:
    HTTP_CODE parseRequestLine(const string &line);
    HTTP_CODE parseHeader(const string &line);
    HTTP_CODE parseBody();

    void parsePath();
    void parseFromUrlEncoded();
    void parseFormData();

    static bool userVerify(const string &name, const string &pwd, bool isLogin);

    PARSE_STATE state;
    string method, path, version, body;
    bool linger;
    size_t contentLen;
    unordered_map<string, string> header;
    unordered_map<string, string> post;
    unordered_map<string, string> fileInfo;

    static const unordered_set<string> DEFAULT_HTML;
    static const unordered_map<string, int> DEFAULT_HTML_TAG;
    static int convertHex(char ch);
};

#endif