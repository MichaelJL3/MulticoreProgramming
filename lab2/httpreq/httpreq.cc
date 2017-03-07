// httpreq.cc - HTTP Request Parser
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

#include <string>
#include <string.h>
#include <iostream>

HTTPReq::HTTPReq(const char *buffer, size_t len)
	: parsed_(false)
	, malformed_(true)
{
	data_ = std::string(buffer, len);
}

// This must be called before any of the accessors are used.
// Returns 0 if the parsing succeeded, a negative value otherwise.
int HTTPReq::parse(void) {
	parsed_ = true;
	size_t content_length = 0;

	// Parse the request header
	size_t start = 0, end;
	end = data_.find(header_sep, start);
	if (end == start || end == std::string::npos) {
		std::cerr << "Failed to find request method/type line" << std::endl;
		return -1;
	}

	char method[255], uri[255];
	int rval;
	if (3 != (rval = sscanf(data_.substr(start, end - start).c_str(), "%254s %254s HTTP/%lf", 
		            method, uri, &version_))) {
		std::cerr << "Failed to scan request method/type from request (got " << rval
		          << " pieces)" << std::endl;
		return -1;
	}
	method_ = method;
	uri_ = uri;
	start = end + header_sep.length();

	// Parse the following header items
	do {
		// Find the next header line
		end = data_.find(header_sep, start);
		if (end == start || end == data_.length() || end == std::string::npos) {
			break;
		}
		std::string line = data_.substr(start, end - start);
		start = end + header_sep.length();

		// Make sure it's valid
		size_t colon = line.find_first_of(':');
		if (colon == std::string::npos) {
			std::cerr << "Malformed request header item: '" << line << "'" << std::endl;
			return -1;
		}

		// Figure out what it contains
		std::string key = line.substr(0, colon - 0);
		std::string val = line.substr(colon + 2);
		if (0 == strncasecmp(key.c_str(), "content-length", key.length())) {
			content_length = strtol(val.c_str(), nullptr, 0);
		} else {
			std::cerr << "Ignoring header field '" << key << "'" << std::endl;
		}

	} while ((start != end) && (end != data_.length() && (end != std::string::npos)));

	// Parse out the body, if we can
	if (start == end && end != data_.length()) {
		start = end + header_sep.length();

		if (content_length > 0) {
			if (start + content_length > data_.length()) {
				std::cerr << "Premature body end (" << start + content_length
				          << " > " << data_.length() << ")" << std::endl;
				return -1;
			}
			body_ = data_.substr(start, content_length);

		} else {
			body_ = "";
		}
	}

	malformed_ = false;
	return 0;
}

const std::string HTTPReq::getMethod(void) const {
	return method_;
}

const std::string HTTPReq::getURI(void) const {
	return uri_;
}

const std::string HTTPReq::getBody(void) const {
	return body_;
}

const double HTTPReq::getVersion(void) const {
	return version_;
}

const bool HTTPReq::isMalformed(void) const {
	return malformed_;
}

std::ostream& operator<<(std::ostream& os, const HTTPReq& req) {
	os << "Header: HTTP version " << req.version_ << ", method " << req.method_
	   << ", uri '" << req.uri_ << "'. Body: " << std::endl;
	os << req.body_ << std::endl;
	return os;
}
