#ifndef __HTTP_REQUEST_MGR_H__
#define __HTTP_REQUEST_MGR_H__

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/multi.h"

#include <semaphore.h>
#include <pthread.h>


#include <map>
#include <vector>
#include <string>

#define HTTP_REQUEST_GET         1
#define HTTP_REQUEST_POST        2
#define HTTP_REQUEST_DELETE      3

#define HTTP_IN_PROCESSING       1
#define HTTP_PROCESS_SUCCESS     0
#define HTTP_PROCESS_FAIL        -1

#define HTTP_PROCESS_TASK (1<<21)

#define HTTP_TIMEOUT             5   /* http请求超时时间5秒 */

#define MAX_HTTP_CONNECTIONS        2048

#define HTTP_RESND_TIMES           1 /* http发送失败后，重试次数 */

/* 从三次握手成功以后，http连接时间设置为30s，超过该时间会关闭 */
#define HTTP_CONNECT_TIMEOUT               10


typedef struct {
	std::string name;
	std::string value;
} Http_Header;

typedef struct {
	int errCode;
	std::string errMsg;
} Http_Err_Info;


typedef void (*http_response_callback)(int handle, int resultCode, const char * data, long datalen, Http_Err_Info *errinfo, void *arg);

typedef struct {

	double downloadSpeed;
	double downloadSize;

}Http_DownLoad_Info;




typedef struct {

	int statusCode;

	std::string responseBody;
	long  bodyLen;

	Http_Err_Info httpErrInfo;

	http_response_callback callback;

} Http_Response;

class HttpRequest {

public:
	std::string url;
	std::string host;
	std::string body;

	bool isDownload;
	long long downloadStartPos;

	std::vector<Http_Header> headerList;

	int requestType; /* Get Or Post */

	int httpTimeoutInterval; //http连接时间，超过该时间，http连接会关闭
	http_response_callback callback;

	int resndTimes;    //发送失败时，重新发送次数

	void * arg; /* 用于在回调函数里面，调用该变量 */

	HttpRequest();
	~HttpRequest();
};

typedef struct {

	int handle;
	CURL *curlHandle;

	Http_Response response;
	struct curl_slist *slist;

	bool isDownload;

	/* http请求发送失败后，重新发送的次数  */
	int alreadyRetryTimes;

	HttpRequest request;


} Http_Session;

class HttpManager
{

public: 

	int send_http_request(HttpRequest *request);

	void cancel_http_request(int handle);
	//Http_DownLoad_Info get_http_download_info(int handle);

	void process_download_data(Http_Session *sess, char *data, long datalen);

	static HttpManager* getInstance();

	~HttpManager();
	HttpManager();
	pthread_t threadid;

private:

	CURLM *curlManager;

	static HttpManager* instance;

	pthread_mutex_t requestListMutex;
	std::vector<Http_Session *> sndRequestList; /* 等待发送的http请求 */

    std::map<int, Http_Session *> sessionList;  /* http session 列表 */

	pthread_mutex_t cancelListMutex;
    std::vector<int> cancelReqList;             /* 等待取消的http请求 */
	sem_t sem;

	void http_process();
	static void *ThreadFunc(void* pThis);
	bool init_curl_easy(Http_Session *sess);
	void prepare_http_request();
	void cancel_request_inline();
	void process_http_response(CURLMsg *msg);
};

#endif
