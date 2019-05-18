/*
 * reauest.h
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <string>

#include "headers.h"

namespace chttp {

enum class Request_Method {
	GET, POST, HEAD, PUT, DELETE, OPTIONS, CONNECT, TRACE, ALL, NONE
};

enum class Http_Version {
	HTTP1_0, HTTP1_1, HTTP2_0, NONE
};

using URL_type=std::string;

class Request final {
public:
	using URL_type=std::string;

	Request() = delete;
	Request(Request_Method method, URL_type url, Http_Version version,
			Headers header) :
			method(method), url(url), version(version), header(header) {
	}
	~Request();

	inline void SetHeader(const Headers& headers) {
		this->header = headers;
	}

	inline const URL_type& URL() const {
		return url;
	}

	inline const Headers& Header() const {
		return header;
	}

	inline Headers& Header() {
		return header;
	}

	inline const Request_Method& Method() const {
		return method;
	}

	inline const Http_Version& Version() const {
		return version;
	}

private:
	Request_Method method;
	URL_type url;
	Http_Version version;
	Headers header { };
};

} /* namespace chttp */

#endif /* REQUEST_H_ */
