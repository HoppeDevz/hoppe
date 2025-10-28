#ifndef HTTP_REQ_BUFFER_PARSER 
#define HTTP_REQ_BUFFER_PARSER

#include <iostream>
#include <string.h>
#include <string>
#include <map>

typedef std::map<std::string, std::string> RequestHeaders;

struct RequestBody {
    int content_length;
    char* data;
};

struct RequestDetails {
    std::string method;
    std::string route;
    std::string http_version;
};

struct HttpRequest {
    RequestDetails  details;
    RequestHeaders  headers;
    RequestBody     body;
};

struct HttpComponentRange {
    int start_at;
    int finish_at;
};

class HttpRequestParser {

    private:

    public:
        static void calculate_components_range(char* buffer, int size, HttpComponentRange* start_line_range,  HttpComponentRange* request_headers_range, HttpComponentRange* body_range);
        static void parse_http_request_buffer(char* buffer, int size, HttpRequest* request);
        static void parse_request_startline(char* startline_buff, int size, RequestDetails* details);
        static void parse_request_headers(char* request_headers_buff, int size, RequestHeaders* headers);
        static void parse_request_body(char* request_body_buff, int size, RequestHeaders* headers, RequestBody* body);

};

#endif