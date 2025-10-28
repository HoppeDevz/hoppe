
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
                request_headers_range->finish_at = i + 2;
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

    RequestBody body{};
    HttpRequestParser::parse_request_body(request_body_buff, request_body_buff_size, &headers, &body);

    request->details = details;
    request->headers = headers;
    request->body = body;

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

    int cursor = 0;
    int reading_type = 0;

    std::string current_key;

    for (int i = 0; i <= size; i++) {

        if (request_headers_buff[i] == 0x3A && reading_type == 0) {

            int key_size = i - cursor;
            char* key_buff = new char[key_size];
            memcpy(key_buff, &request_headers_buff[cursor], key_size);

            current_key = std::string(key_buff, key_size);
            cursor = i + 2;
            reading_type = 1;

            free(key_buff);

        }

        if (request_headers_buff[i] == 0x0D && reading_type == 1) {

            int value_size = i - cursor;
            char* value_buff = new char[value_size];
            memcpy(value_buff, &request_headers_buff[cursor], value_size);

            reading_type = 0;
            cursor = i + 2;

            headers->emplace(current_key, std::string(value_buff, value_size));
            free(value_buff);

        }

    }

}

void HttpRequestParser::parse_request_body(char* request_body_buff, int size, RequestHeaders* request_headers, RequestBody* body) {


    RequestHeaders::iterator it = request_headers->find("Content-Length");

    if (it == request_headers->end())
        return;

    int content_length = std::stoi(it->second);
    std::string body_data = std::string(request_body_buff, content_length);

    body->content_length = content_length;
    body->data = (char*)malloc(content_length);

    memcpy(body->data, request_body_buff, content_length);
    
}
