#include "http_request_mgr.h"
#include <string>
#include <iostream>  
#include <pthread.h>

using namespace std;      

static pthread_mutex_t g_http_init_mutext = PTHREAD_MUTEX_INITIALIZER;

static void lock_instance_init()
{
	pthread_mutex_lock(&g_http_init_mutext);
}

static void unlock_instance_init()
{
	pthread_mutex_unlock(&g_http_init_mutext);
}

static unsigned int handle_global = 0;
HttpRequest::HttpRequest()
{

	callback = NULL;
	requestType = -1;
	isDownload = false;
	downloadStartPos = -1;
	httpTimeoutInterval = 0;

	resndTimes = 0;

	host = "";

	arg = NULL;
}

HttpRequest::~HttpRequest()
{
}

HttpManager* HttpManager::instance = NULL;

// w finished:
HttpManager::HttpManager()
{

	curl_global_init(CURL_GLOBAL_ALL);
	curlManager = curl_multi_init();

    pthread_mutex_init(&requestListMutex, NULL); //PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&cancelListMutex, NULL);  //PTHREAD_MUTEX_INITIALIZER;


	//curl_multi_setopt(curlManager, CURLMOPT_MAXCONNECTS, (long)MAX_HTTP_CONNECTIONS);
	//printf("init HttpManager============================\n");

    //If pshared has the value 0, then the semaphore is shared between the threads of a process, and should be located at some address  that
    //is visible to all threads (e.g., a global variable, or a variable allocated dynamically on the heap).

	sem_init(&sem, 0, 0);

	pthread_create( &threadid, NULL, &ThreadFunc, this );
	//pthread_join( threadid, NULL );


}

HttpManager* HttpManager::getInstance()
{
	lock_instance_init();
	if(NULL == instance) {
		instance = new HttpManager();
	}
	unlock_instance_init();

	return instance;
}

HttpManager::~HttpManager()
{
	curl_global_cleanup();

}
// w finished:
void HttpManager::process_download_data(Http_Session *sess, char *data, long datalen)
{

	bool isCancel = false;
	int index;

	/* 为了调用取消请求函数执行完毕以后，下载不 再返回数据，此处加锁 */
	pthread_mutex_lock( &cancelListMutex );

	if(cancelReqList.size() > 0) {

		for(index = 0; index < (int)(cancelReqList.size()); index++) {

			if(cancelReqList[index] == sess->handle) {
				isCancel = true;
				break;
			}
		}
	}
	if(!isCancel) {
		//printf("@@@@@@@@@@@@@@rcv data len[%ld]\n", datalen);
        /* 获取信源信息的回调函数
        * void GetSingerList::callback_for_SingerList
        */
  //void (*http_response_callback)(int handle, int resultCode, const char * data, long datalen, Http_Err_Info *errinfo, void *arg);
        printf("sess->handle is %d, data is %s, datalen is %ld \n",sess->handle, data, datalen);
        sess->response.callback(sess->handle, HTTP_IN_PROCESSING, data, (long)datalen, NULL, sess->request.arg);
	}

	pthread_mutex_unlock( &cancelListMutex );

}

static size_t rcv_headers( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	//Http_Session *sess = (Http_Session *)userdata;
	//printf("header[%s]\n", (char *)ptr);

	/* 在此处获取http请求返回结果的所有header */

	return  size * nmemb;
}

static size_t write_callback(char *data, size_t size, size_t nmemb, void *userdata)
{
	size_t datalen = size * nmemb;

	Http_Session *sess = (Http_Session *)userdata;
	Http_Response *response = &(sess->response);

	//printf("@@@@@@@@@@@@ rcv data len[%d] handle[%d]\n", datalen, sess->handle);

	if(sess->isDownload) {
		
		HttpManager::getInstance()->process_download_data(sess, data, datalen);
	} else {
		response->bodyLen += datalen;
		response->responseBody.append(data, datalen);
	}


	return datalen;
}
// w finished:
void HttpManager::cancel_http_request(int handle)
{
	//printf("run cancel handle[%d] #####################\n", handle);
	pthread_mutex_lock( &cancelListMutex );
	cancelReqList.push_back(handle);
	pthread_mutex_unlock( &cancelListMutex );
	//cancel_request_inline();
	
}

/*
Http_DownLoad_Info HttpManager::get_http_download_info(int handle)
{
	Http_DownLoad_Info http_download_info;

	map<int, Http_Session *>::iterator it;

	requestListMutex.Lock();
	it = sessionList.find(handle);

	if (it == sessionList.end()){

		http_download_info.downloadSize = 0;
		http_download_info.downloadSize = 0;

	} else {

		if(NULL != it->second)
		curl_easy_getinfo(it->second->curlHandle, CURLINFO_SPEED_DOWNLOAD, &(http_download_info.downloadSize));
		curl_easy_getinfo(it->second->curlHandle, CURLINFO_SIZE_DOWNLOAD, &(http_download_info.downloadSize));

	}
	requestListMutex.Unlock();

	return http_download_info;
}*/
// w finished:
int HttpManager::send_http_request(HttpRequest *request)    // send request,  step 1
{
	/* url为空的时候直接返回 */
	if(request->url.length() <= 0) {
		printf("[HTTP] url must not be NULL\n");
		return -1;
	}

	Http_Session *sess = new Http_Session;

    //memcpy(&(sess->request), request, sizeof(HttpRequest));

	sess->response.callback = request->callback;

	sess->request.url = request->url;
	sess->request.host = request->host;
	sess->request.body = request->body;
	printf("############## body[%s]\n", sess->request.body.c_str());

	sess->request.httpTimeoutInterval = request->httpTimeoutInterval;
	sess->request.downloadStartPos = request->downloadStartPos;
	sess->request.requestType = request->requestType;
	sess->request.resndTimes = request->resndTimes;
	sess->alreadyRetryTimes = 0;
	sess->request.arg = request->arg;

	/* http header保存 */
	for(unsigned index = 0; index < request->headerList.size(); index++) {
		sess->request.headerList.push_back(request->headerList[index]);

	}

	sess->slist = NULL;
	sess->curlHandle = NULL;

	sess->isDownload = request->isDownload;

	sess->response.responseBody = "";
	sess->response.bodyLen = 0;

	/* 获取http的handle */
	lock_instance_init();
	handle_global++;
	sess->handle  = handle_global;
	unlock_instance_init();

	/* 把http请求放入list，等待发送 */
	pthread_mutex_lock( &requestListMutex );
	sndRequestList.push_back(sess);
	pthread_mutex_unlock( &requestListMutex );

	printf("@@@@@@@@@@@@@@@@@@@@run handle[%d]\n", sess->handle);

	/*激活信号量，发送http请求*/
	sem_post( &sem );

	return sess->handle;
}

void* HttpManager::ThreadFunc(void* pThis)
{
	if(pThis == NULL)
	{
		return NULL;
	}
	((HttpManager*)pThis)->http_process();

	return NULL;
}

// w finished:
bool HttpManager::init_curl_easy(Http_Session *sess)
{
	HttpRequest *request = &(sess->request);
	sess->curlHandle = curl_easy_init();
	if (!sess->curlHandle)
	{
		printf("[HTTP] create curl easy handle failed.\n");
		delete sess;
		return false;
	}

	// 设置sharehandle
	//curl_easy_setopt(request->curlHandle, CURLOPT_SHARE, g_HTTPConnection->getShareHandle());
	// 设置远端地址
	if (request->url.length() > 0)
	{
		//printf("url[%s]\n",  request->url.c_str());
		curl_easy_setopt(sess->curlHandle, CURLOPT_URL, request->url.c_str());
	}
	else
	{
		printf("[HTTP] url must not be NULL\n");
		//printf("[HTTP] url must not be NULL\n");
		delete sess;
		return false;
	}

	if(request->requestType == HTTP_REQUEST_POST) {
		curl_easy_setopt(sess->curlHandle, CURLOPT_POST, 1L);

		//* 设置POST请求的body部 *
		if (request->body.length() > 0)
		{
			printf("@@@@@@@@@@@@@@ body[%s]\n", request->body.data());
			curl_easy_setopt(sess->curlHandle, CURLOPT_POSTFIELDSIZE, request->body.length());
			curl_easy_setopt(sess->curlHandle, CURLOPT_COPYPOSTFIELDS, request->body.c_str());
		}
	}

	if(request->requestType == HTTP_REQUEST_DELETE) {
		curl_easy_setopt(sess->curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");

	}

	/* 设置http请求的header */
	if(request->headerList.size() > 0 || request->host.length() > 0) {

		string header;
		Http_Header *tmpHdr;
		int index;

		for(index = 0; index < (int)(request->headerList.size()); index++)
		{
			header.clear();
			if(index > 0)
			{
				header.append("&");
			}
			tmpHdr = &(request->headerList[index]);
			header.append(tmpHdr->name);
			header.append("=");
			header.append(tmpHdr->value);
			
			printf("@@@@@@@@@@@ add header[%s]\n",  header.c_str());

			sess->slist = curl_slist_append(sess->slist, header.c_str()); 

		}
		if(request->host.length() > 0) {

			header.clear();
			header.append("Host: ");
			header.append(request->host);
			sess->slist = curl_slist_append(sess->slist, header.c_str()); 
		}

		curl_easy_setopt(sess->curlHandle, CURLOPT_HTTPHEADER, sess->slist); 

	} else {
		sess->slist = NULL;
	}

	//if(request->downloadStartPos > 0) {
	//	char down_range[128] = {0};
	//	sprintf(down_range, "%ld-", request->downloadStartPos);
	//	curl_easy_setopt(request->curlHandle, CURLOPT_RANGE, down_range);
	//}

	curl_easy_setopt(sess->curlHandle, CURLOPT_SSL_VERIFYPEER, 0);

	curl_easy_setopt(sess->curlHandle, CURLOPT_SSL_VERIFYHOST, 0);

	curl_easy_setopt(sess->curlHandle, CURLOPT_WRITEFUNCTION, write_callback);

	curl_easy_setopt(sess->curlHandle, CURLOPT_WRITEDATA, sess);

	curl_easy_setopt(sess->curlHandle, CURLOPT_PRIVATE, sess);

	//* 调试用开关，设为1的时候，会在标准输出里面打印发送请求和返回结果的内容  *
	curl_easy_setopt(sess->curlHandle, CURLOPT_VERBOSE, 0L);


	curl_easy_setopt(sess->curlHandle, CURLOPT_FAILONERROR, 1);

	//* 设置tcp三次握手超时时间 */
	curl_easy_setopt(sess->curlHandle, CURLOPT_CONNECTTIMEOUT, 15L);


	//* 获取http请求返回结果中的header *
	curl_easy_setopt(sess->curlHandle, CURLOPT_HEADERFUNCTION, rcv_headers);
	curl_easy_setopt(sess->curlHandle, CURLOPT_WRITEHEADER, sess);

	//* 设置http连接时间，超过该时间，则会断开http连接 *
	curl_easy_setopt(sess->curlHandle, CURLOPT_TIMEOUT, request->httpTimeoutInterval);


	curl_easy_setopt(sess->curlHandle, CURLOPT_NOSIGNAL, 1);

	//* 由于始终使用同一个连接的端口，发送数据。长时间后该端口会不可用，所以禁止端口重用 *
	curl_easy_setopt(sess->curlHandle, CURLOPT_FORBID_REUSE, 1L);

	return true;
}

void HttpManager::prepare_http_request()
{
	unsigned int index;
	Http_Session *sess;
	bool result;

	/* 把http请求发给libcurl */
	pthread_mutex_lock( &requestListMutex );

	if(sndRequestList.size() > 0) {
		for(index = 0; index < sndRequestList.size(); index++) {

			sess = sndRequestList[index];
			
			result = init_curl_easy(sess);
			if(result) {
				curl_multi_add_handle(curlManager, sess->curlHandle);
				sessionList.insert(map<int, Http_Session *>::value_type(sess->handle, sess));

				//printf("run handle[%d], time[%ld]\n", sess->handle, (long)time(NULL));
			} else {
				printf("[HTTP] HTTP request init failed. url[%s]\n", sess->request.url.c_str());
				//printf("[HTTP] HTTP request init failed. url[%s]\n", sess->request.url.c_str());
			}
		}
		sndRequestList.clear();
	}

	pthread_mutex_unlock( &requestListMutex );
}

void HttpManager::cancel_request_inline()
{
	unsigned int index;
	map<int, Http_Session *>::iterator it;
	Http_Session *sess;

	pthread_mutex_lock( &cancelListMutex );
	if(cancelReqList.size() > 0) {

		for(index = 0; index < cancelReqList.size(); index++) {

			it = sessionList.find(cancelReqList[index]);
			if (it != sessionList.end()){

				sess = it->second;

				/* 将请求从libcurl模块移除，并释放申请的内存 */
				curl_multi_remove_handle(curlManager,sess->curlHandle);
				curl_easy_cleanup(sess->curlHandle);
				if(sess->slist)
					curl_slist_free_all(sess->slist);

				sessionList.erase(it);
				delete sess;
			}

		}
		cancelReqList.clear();
	}

	pthread_mutex_unlock( &cancelListMutex );
}

void HttpManager::process_http_response(CURLMsg *msg)
{
	int index;
	int ret;
	map<int, Http_Session *>::iterator it;
	Http_Session *sess;

	bool isCancel = false;
	bool isResend = false;

	/* 为了调用取消请求函数执行完毕以后，下载不再返回数据，此处加锁 */

	pthread_mutex_lock( &cancelListMutex );
	ret = curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &sess);

	if(ret == CURLE_OK ) {

		if(cancelReqList.size() > 0) {

			/* 检查当前请求是否取消 */
			for(index = 0; index < (int)(cancelReqList.size()); index++) {

				if(cancelReqList[index] == sess->handle) {
					isCancel = true;
					break;
				}
			}
		}

		if(msg->data.result ==  CURLE_OK && !isCancel) {

			sess->response.callback(sess->handle, HTTP_PROCESS_SUCCESS, sess->response.responseBody.data(), sess->response.bodyLen, NULL, sess->request.arg);

		}else {

			if(!isCancel) {

				//curl_easy_getinfo(msg->easy_handle, CURLINFO_LOCAL_PORT, &srcport); 
				//curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIMARY_PORT, &dstport);

				//err info output
				printf("[HTTP] request error, curl return code: %d, %s\n", msg->data.result, curl_easy_strerror(msg->data.result));

				if(sess->request.resndTimes > 0) {

					/* 重新发送http请求 */
					if( sess->alreadyRetryTimes < sess->request.resndTimes ) {

						curl_multi_remove_handle(curlManager, sess->curlHandle);
						curl_multi_add_handle(curlManager, sess->curlHandle);
						sess->alreadyRetryTimes++;
						printf("############# Resend http request, handle[%d], resend times[%d], already resend times[%d]\n",
								sess->handle, sess->request.resndTimes , sess->alreadyRetryTimes);

						isResend = true;

					} else {

						/* 重发失败达到最大次数，调用回调函数返回 */
						printf("@@@@@@@@@@@ CallBack run for err http handle[%d]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n", sess->handle);

						Http_Err_Info errinfo;
						errinfo.errCode = msg->data.result;
						errinfo.errMsg = curl_easy_strerror(msg->data.result);
						sess->response.callback(sess->handle, HTTP_PROCESS_FAIL, sess->response.responseBody.data(), sess->response.bodyLen, &errinfo, sess->request.arg);

					}

				} else {

					/* 不重发的时候直接调用回调函数返回 */
					printf("@@@@@@@@@@@ CallBack run for err http handle[%d]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2\n", sess->handle);

					Http_Err_Info errinfo;
					errinfo.errCode = msg->data.result;
					errinfo.errMsg = curl_easy_strerror(msg->data.result);
					sess->response.callback(sess->handle, HTTP_PROCESS_FAIL, sess->response.responseBody.data(), sess->response.bodyLen, &errinfo, sess->request.arg);
				}
			}
		}
	}
	pthread_mutex_unlock( &cancelListMutex );
	/* 如果是重新发送则不删除session信息，函数返回 */
	if(isResend) {
		sem_post( &sem );
		return;
	}

	//printf("@@@@@@@@@@@@@@@@@@@@@@release handle[%d]@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", sess->handle);

	/* 当前请求执行完毕，从libcurl中删除，并清空该请求所占用的内存 */
	curl_multi_remove_handle(curlManager, sess->curlHandle);
	curl_easy_cleanup(sess->curlHandle);
	if(sess->slist)
		curl_slist_free_all(sess->slist);

	/* 将session信息从列表中删除，并释放保存的关于当前http请求的所有信息 */
	it = sessionList.find(sess->handle);
	if (it != sessionList.end()){
		sessionList.erase(it);
	}

	delete sess;
	sess = NULL;

}

/* 发送http请求，接收返回数据 */
void HttpManager::http_process()
{

	int still_running = 0;
	struct timeval timeout;
	int rc; /* select() return code */ 
	CURLMcode mc; /* curl_multi_fdset() return code */ 

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int maxfd = -1;
	long curl_timeo;


	int msgs_left;
	CURLMsg *msg;

	map<int, Http_Session *>::iterator it;

	while(true){
		/* 信号量没有激活的时候，发送http请求的线程将处于挂起状态，不占用系统cpu */
		sem_wait( &sem );

		/* 将http请求传递libcurl */
		prepare_http_request();

		curl_multi_perform(curlManager, &still_running);

		do{
			if(still_running > 0) {
				curl_timeo = -1;
				maxfd = -1;

				FD_ZERO(&fdread);
				FD_ZERO(&fdwrite);
				FD_ZERO(&fdexcep);

				/* set a suitable timeout to play around with */ 
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;

				curl_multi_timeout(curlManager, &curl_timeo);
				if(curl_timeo >= 0) {
					timeout.tv_sec = curl_timeo / 1000;
					if(timeout.tv_sec > 1)
						timeout.tv_sec = 1;
					else
						timeout.tv_usec = (curl_timeo % 1000) * 1000;
				}

				/* get file descriptors from the transfers */ 
				mc = curl_multi_fdset(curlManager, &fdread, &fdwrite, &fdexcep, &maxfd);

				if(mc != CURLM_OK) {
					printf("curl_multi_fdset() failed, code %d.\n", mc);
					break;
				}

				/* On success the value of maxfd is guaranteed to be >= -1. We call
				select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
				no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
				to sleep 100ms, which is the minimum suggested value in the
				curl_multi_fdset() doc. */ 

				if(maxfd == -1) {
#ifdef _WIN32
					Sleep(100);
					rc = 0;
#else
					/* Portable sleep for platforms other than Windows. */ 
					struct timeval wait = { 0, 100 * 1000 }; /* 100ms */ 
					rc = select(0, NULL, NULL, NULL, &wait);
#endif
				}
				else {
					/* Note that on some platforms 'timeout' may be modified by select().
					If you need access to the original value save a copy beforehand. */ 
					//printf("select time[%d.%ld]\n", timeout.tv_sec, timeout.tv_usec);
					rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
				}

				switch(rc) {
					case -1:
						/* select error */ 
						break;
					case 0:
					default:
						/* timeout or readable/writable sockets */ 
						curl_multi_perform(curlManager, &still_running);
						break;
				}

				//printf("run curl_multi_perform still runing ===============================\n");

			}
			//printf(" **************** run curl_multi_perform *********************\n");

			cancel_request_inline();

			while((msg = curl_multi_info_read(curlManager, &msgs_left))) {
				if(msg->msg == CURLMSG_DONE) {
					process_http_response(msg);
				}
			}

			/* 将http请求传递libcurl */
			prepare_http_request();


		} while(still_running);
	}
	pthread_exit(NULL);
}

