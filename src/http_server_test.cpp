#include "http_server.h"
#include "event_loop.h"
#include "logging.h"

void on_request(const tinynet::HttpRequest &request, tinynet::HttpResponse &response)
{
    LOG(DEBUG) << "recv URL:" <<request.get_url() << std::endl; 
    LOG(DEBUG) << "recv headers:" << request.dump_heads() << std::endl;
    LOG(DEBUG) << "recv body:" << request.get_body() << std::endl;
    if (request.get_method() == tinynet::HttpRequest::GET)
    {
        response.set_body("Hello, world! TEST GET");
    }
    else if(request.get_method() == tinynet::HttpRequest::POST)
    {
        response.set_body("Hello, world! TEST POST");
    }
    else
    {
        response.set_status(501, "Not Implemented");
        response.set_body("Unsupported method");
    }
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::HttpServer server(&event_loop, "172.29.51.204", 8070, "http_test_server");
    server.set_onrequest_cb(on_request);
    server.start();
    event_loop.loop();

    return 0;
}