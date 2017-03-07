// test-httpreq.cc - Unit Test for HTTP request parser / response generator
// (c) 2017 Christopher Mitchell, Ph.D.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "httpreq.hpp"
#include "httpresp.hpp"
#include <iostream>
#include <string.h>

struct HTTPReqSample {
	const char* request;
	const char* method;
	const char* uri;
	const double version;
	const char* body;
};

const size_t n_req_samples = 4;
struct HTTPReqSample httpreq_samples[n_req_samples] = {
	{"POST /cgi-bin/process.cgi HTTP/1.1\r\n"
	 "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	 "Host: www.tutorialspoint.com\r\n"
	 "Content-Type: application/x-www-form-urlencoded\r\n"
	 "Content-Length: 49\r\n"
	 "Accept-Language: en-us\r\n"
	 "Accept-Encoding: gzip, deflate\r\n"
	 "Connection: Keep-Alive\r\n"
	 "\r\n"
	 "licenseID=string&content=string&/paramsXML=string\r\n",
	 "POST", "/cgi-bin/process.cgi", 1.1,
	 "licenseID=string&content=string&/paramsXML=string"},

	{"GET /user1024 HTTP/1.1\r\n"
	 "SomeHeader: Random value here\r\n"
	 "AnotherHeader: Random value here\r\n"
	 "\r\n",
	 "GET", "/user1024", 1.1, ""},

	{"POST /user2048 HTTP/1.1\r\n"
	 "SomeHeader: Random value here\r\n"
	 "Content-length: 10\r\n"
	 "AnotherHeader: Random value here\r\n"
	 "\r\n"
	 "Abcdefghij\r\n",
	 "POST", "/user2048", 1.1, "Abcdefghij"},
	
	{"DELETE /user65535 HTTP/1.1\r\n"
	 "SomeHeader: Random value here\r\n"
	 "AnotherHeader: Random value here\r\n"
	 "\r\n",
	 "DELETE", "/user65535", 1.1, ""},

};

struct HTTPRespSample {
	const char* body;
	const unsigned int code;
	const bool keep_alive;
	const char* response;
};

const size_t n_resp_samples = 3;
struct HTTPRespSample httpresp_samples[n_resp_samples] = {
	{"<html>\r\n"
	 "<body>\r\n"
	 "<h1>Hello, World!</h1>\r\n"
	 "</body>\r\n"
	 "</html>\r\n",
	 200,
	 false,
	 "HTTP/1.1 200 OK\r\n"
	 "Content-Length: 58\r\n"
	 "Connection: close\r\n"
	 "\r\n"
	 "<html>\r\n"
	 "<body>\r\n"
	 "<h1>Hello, World!</h1>\r\n"
	 "</body>\r\n"
	 "</html>\r\n"},

	{"Page not found!",
	 404,
	 true,
	 "HTTP/1.1 404 Not Found\r\n"
	 "Content-Length: 15\r\n"
	 "Connection: keep-alive\r\n"
	 "\r\n"
	 "Page not found!"},

	{"Server error!",
	 500,
	 true,
	 "HTTP/1.1 500 Internal Server Error\r\n"
	 "Content-Length: 13\r\n"
	 "Connection: keep-alive\r\n"
	 "\r\n"
	 "Server error!"}
};

int main(const int argc, const char* argv[]) {
	size_t passed = 0;

	// Test the HTTPReq class
	for(size_t i = 0; i < n_req_samples; i++) {
		HTTPReq request(httpreq_samples[i].request, strlen(httpreq_samples[i].request));
		if (0 != request.parse() || request.isMalformed()) {
			std::cerr << "Failed to parse sample request #" << i << "; aborting test" << std::endl;
			return -1;
		}
		std::cout << request << std::endl;
		if (request.getMethod() != httpreq_samples[i].method ||
		    request.getURI() != httpreq_samples[i].uri ||
		    request.getVersion() != httpreq_samples[i].version ||
		    request.getBody() != httpreq_samples[i].body)
		{
			std::cerr << "Parsed request #" << i << " improperly; aborting test" << std::endl;
		}
		passed++;
	}

	// Test the HTTPResp class
	for(size_t i = 0; i < n_resp_samples; i++) {
		HTTPResp response(httpresp_samples[i].code, httpresp_samples[i].body, httpresp_samples[i].keep_alive);
		if (response.isMalformed()) {
			std::cerr << "Failed to build response #" << i << "; aborting test" << std::endl;
			return -1;
		}
		const std::string raw_response = response.getResponse();
		std::cout << "'" << raw_response << "'" << std::endl;
		if (raw_response != httpresp_samples[i].response) {
			std::cerr << "Built response #" << i << " improperly; aborting test" << std::endl;
			return -1;
		}
		passed++;
	}

	if (passed == n_req_samples + n_resp_samples) {
		std::cerr << "All tests passed!" << std::endl;
	}
	return 0;
}
