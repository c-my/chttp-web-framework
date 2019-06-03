/*
 * router.h
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include <unordered_map>
#include <functional>

#include "request.h"
#include "response.h"

namespace chttp {

struct RequestInfo {
	Request::URL_type url;
	Request_Method method;
	RequestInfo(Request::URL_type url, Request_Method method) :
			url(url), method(method) {
	}
	bool operator ==(const RequestInfo& other) const {
		return (this->url == other.url) && (this->method == other.method);
	}
};

struct RequestInfoHasher {
	std::size_t operator()(const RequestInfo & info) const {
		using std::hash;
		return (hash<std::string>()(info.url.GetURL())
				^ hash<int>()(static_cast<int>(info.method)));
	}
};

class Router final {
public:
	using Handler=std::function<void(Request&, Response&)>;
	using Handler_table=std::unordered_map<RequestInfo, Handler, RequestInfoHasher>;

	Router() = default;
	virtual ~Router();

	// there should be a out-of-range exception when no handler
	inline const Handler GetHandler(const Request::URL_type& url,
			Request_Method method) const {
		return handler_table.at(RequestInfo(url, method));
	}

	inline const Handler GetHandler(const RequestInfo& info) const {
		return handler_table.at(info);
	}

	inline bool Get(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::GET, handler);
	}

	inline bool Head(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::HEAD, handler);
	}

	inline bool Post(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::POST, handler);
	}

	inline bool Put(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::PUT, handler);
	}

	inline bool Delete(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::DELETE_M, handler);
	}

	inline bool Connect(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::CONNECT, handler);
	}

	inline bool Options(const Request::URL_type& url, Handler handler) {
		return AddHandler(url, Request_Method::OPTIONS, handler);
	}

	inline bool Trace(const Request::URL_type& url,
			Handler handler) {
		return AddHandler(url, Request_Method::TRACE, handler);
	}

	inline bool AddHandler(const Request::URL_type& url, Request_Method method,
			Handler handler) {
		auto result = handler_table.insert(
				std::make_pair(RequestInfo(url, method), handler));
		return result.second;
	}

	inline bool Contains(const Request::URL_type & url,
			const Request_Method & method) const {
		return Contains( { url, method });
	}

	inline bool Contains(const RequestInfo & info) const {
		return handler_table.find(info) != handler_table.end();
	}

private:
	Handler_table handler_table { };
};

} /* namespace chttp */

#endif /* ROUTER_H_ */
