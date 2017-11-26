#include <stdio.h>
#include "http_request_mgr.h"
#include "SignatureGenerator.h"
#include <cstring>
#include <vector>

using namespace std;

//this is a callback function for httpRequest: context category
/*
typedef struct {
	int errCode;
	std::string errMsg;
} Http_Err_Info;
 */
void callback1(int handle, int resultCode, const char * data, long datalen, Http_Err_Info *errinfo, void *arg){
	if(resultCode > HTTP_PROCESS_FAIL) 
		printf("handle: %d\nresultCode: %d\ncategory: %s\n\n",handle, resultCode, data);
	else {
		printf("errCode: %d\nerrMsg: %s\n",errinfo->errCode,errinfo->errMsg.c_str());
	}
}

//this is a callback function for httpRequest: context sublist category
void callback2(int handle, int resultCode, const char * data, long datalen, Http_Err_Info *errinfo, void *arg){
	printf("handle: %d\nresultCode: %d\nsublist: %s\n\n",handle, resultCode, data);
}

int main(int argc,char **argv){
	printf("program start!\n");
	HttpRequest *request = new HttpRequest();
	string url = "http://autoapi.openspeech.cn/csp/api/v2.2/programe/type?";
	string openkey = "***";
	string sig = "";
	vector<paraPair> paraList;
	paraPair tmp;
	// common params
	tmp.name = "openId";
	tmp.value = "***";
	paraList.push_back(tmp);

	tmp.name = "sId";
	tmp.value = "0000000000000003";
	paraList.push_back(tmp);

	tmp.name = "clientType";
	tmp.value = "0";
	paraList.push_back(tmp);
	
	// request param
	tmp.name = "pageSize";
	tmp.value = "4";
	paraList.push_back(tmp);

	SignatureGenerator *sc = new SignatureGenerator();
	sc->getSignature(paraList[0].value, openkey, paraList, sig);
	delete sc;

	tmp.name = "sign";
	tmp.value = sig;
	paraList.push_back(tmp);

	for(int i=0; i<paraList.size(); i++){
		if(i > 0){
			url = url+"&";
		}	
		url = url+paraList[i].name;
		url = url+"=";
		url = url+paraList[i].value;
	}

	HttpManager *myHttp = new HttpManager();
	void *ret; 
	int handle;

	request->url = url;
	request->isDownload = false;
	request->httpTimeoutInterval = HTTP_CONNECT_TIMEOUT;
	request->requestType = HTTP_REQUEST_GET;
	request->resndTimes = 1;
	request->callback = callback1;
	request->arg = NULL;	//the last param for callback
	printf("url: %s\n",request->url.c_str());
	handle = myHttp->send_http_request(request);
	if(handle <= 0)
	{
		printf("Http Request[%s] snd failed\n", request->url.c_str());
	}
	else
	{
		printf("HttpManager send request ok.\n");
	}
	
	delete request;
	request = NULL;
	pthread_t tid = myHttp->threadid;
	pthread_join(tid,&ret);
	//pthread_cancel(tid);
	return 0;
}
