/*
 * response.h
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <cstdio>
#include <cstring>

#ifdef __linux__
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <WinSock2.h>
#endif

#include "headers.h"

namespace chttp {

struct HttpStatus {
	constexpr static char OK[] = "200 OK";
	constexpr static char BadRequest[] = "400 Bad Request";
	constexpr static char Unauthorized[] = "401 Unauthorized";
	constexpr static char Forbidden[] = "403 Forbidden";
	constexpr static char NotFound[] = "404 Not Found";
	constexpr static char Unavailable[] = "503 Server Unavailable";
};

struct HttpVersionStr {
	constexpr static char Http1_0[] = "HTTP/1.0";
	constexpr static char Http1_1[] = "HTTP/1.1";
	constexpr static char Http2_0[] = "HTTP/2.0";
};

class Response final {
public:
#ifdef __linux__
	using Port_type = in_port_t;
	using Sock_type = int;
#endif

#ifdef _MSC_VER
	using Port_type = USHORT;
	using Sock_type = UINT_PTR;
#endif

	Response() = delete;
	Response(Sock_type clnt_sock) :
			clnt_sock(clnt_sock) {
	}
	~Response();

	void ServeHtml(const char * file_path);

	void NotFound();

	inline void SetHeaders(const Headers& headers) {
		header = headers;
	}

	inline void SetContentType(std::string&& content_type) {
		header.Content()["Content-Type"] = content_type;
	}

private:
	friend class Server;
#ifdef __linux__
	int clnt_sock;
#elif _MSC_VER
	SOCKET clnt_sock;
#endif // __linux__


	static constexpr int buffer_size { 1024 };
	Headers header { };

	static std::string NotFoundPage;

	void SendStatusLine(const char* http_version, const char* http_status);
	void SendHeader(const Headers& headers);
	void SendFile(int clnt_sock, const char* file_path);

	void WriteSocket(Port_type clnt_sock, const char* buffer, size_t size = 0,
			int flags = 0);
	inline void CloseSocket(Sock_type sock);
};

} /* namespace chttp */

#endif /* RESPONSE_H_ */
