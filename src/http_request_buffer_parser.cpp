
#include <http_request_buffer_parser.hpp>

void HttpRequestParser::calculate_components_range(
    
    char* buffer, 
    int size,

    HttpComponentRange* start_line_range,  
    HttpComponentRange* request_headers_range, 
    HttpComponentRange* body_range

) {

    bool first_line = false;

    for (int i = 0; i < size; i++) {

        if (buffer[i] == 0x0A && !first_line) {

            if (start_line_range != nullptr) {
                start_line_range->start_at = 0;
                start_line_range->finish_at = i + 1;
            }

            if (request_headers_range != nullptr) {
                request_headers_range->start_at = i + 1;
            }

            first_line = true;

        }

        if (
            buffer[i]       == 0x0D &&
            buffer[i + 1]   == 0x0A && 
            buffer[i + 2]   == 0x0D &&
            buffer[i + 3]   == 0x0A
        ) {

            if (request_headers_range != nullptr) {
                request_headers_range->finish_at = i - 1;
            }

            if (body_range != nullptr) {
                body_range->start_at = i + 4;
                body_range->finish_at = size;
            }

        }

    }
    
}

void HttpRequestParser::parse_http_request_buffer(char* buffer, int size, HttpRequest* request) {

    HttpComponentRange start_line_range{};
    HttpComponentRange request_headers_range{};
    HttpComponentRange request_body_range{};
    HttpRequestParser::calculate_components_range(buffer, size, &start_line_range, &request_headers_range, &request_body_range);

    int start_line_buff_size = start_line_range.finish_at - start_line_range.start_at;
    char* start_line_buff = (char*)malloc(start_line_buff_size); memcpy(start_line_buff, &buffer[start_line_range.start_at], start_line_buff_size);

    int request_headers_buff_size = request_headers_range.finish_at - request_headers_range.start_at;
    char* request_headers_buff = (char*)malloc(request_headers_buff_size); memcpy(request_headers_buff, &buffer[request_headers_range.start_at], request_headers_buff_size);

    int request_body_buff_size = request_body_range.finish_at - request_body_range.start_at;
    char* request_body_buff = (char*)malloc(request_body_buff_size); memcpy(request_body_buff, &buffer[request_body_range.start_at], request_body_buff_size);

    RequestDetails details{};
    HttpRequestParser::parse_request_startline(start_line_buff, start_line_buff_size, &details);

    RequestHeaders headers{};
    HttpRequestParser::parse_request_headers(request_headers_buff, request_headers_buff_size, &headers);

    request->details = details;
    request->headers = headers;

}

void HttpRequestParser::parse_request_startline(char* startline_buff, int size, RequestDetails* details) {

    int itemidx = 0;
    int cursor = 0;

    for (int i = 0; i < size; i++) {

        if (startline_buff[i] == 0x20 || startline_buff[i] == 0x0A) {

            int item_size = (i - cursor);
            char* buff = new char[item_size + 1];
            memcpy(buff, &startline_buff[cursor], item_size);
            buff[item_size + 1] = '\0';

            if (itemidx == 0) {
                details->method = std::string(buff, item_size);
            }
            if (itemidx == 1) {
                details->route = std::string(buff, item_size);
            }
            if (itemidx == 2) {
                details->http_version = std::string(buff, item_size);
            }

            cursor = i + 1;
            itemidx++;
            free(buff);

        }

    }

}

void HttpRequestParser::parse_request_headers(char* request_headers_buff, int size, RequestHeaders* headers) {

    

}
