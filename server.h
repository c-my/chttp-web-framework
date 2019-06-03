/*
 * socket.h
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <vector>
#include <utility>
#include <tuple>
#include <string>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstdio>
#include <cstring>

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif // __linux__

#ifdef _MSC_VER
#include <WinSock2.h>
#endif // _MSC_VER


#include "request.h"
#include "response.h"
#include "router.h"

namespace chttp {

class Server final {
public:
#ifdef __linux__
	using Port_type = in_port_t;
	using Sock_type = int;
#endif

#ifdef _MSC_VER
	using Port_type = USHORT;
	using Sock_type = SOCKET;
#endif
	Server();
	~Server();

	void Test(const Port_type& port) {

		auto serv_sock = StartUpSocket(port);
		std::cout << "listening on port:" << port << std::endl;
		struct sockaddr_in clnt_addr;
		while (true) {
#ifdef __linux__
			unsigned int clnt_addr_len = sizeof(clnt_addr);
#elif _MSC_VER
			int clnt_addr_len = sizeof(clnt_addr);

#endif
			Sock_type clnt_sock = accept(serv_sock, (sockaddr*)& clnt_addr,
				&clnt_addr_len);
//			auto clnt_ip = inet_ntoa(clnt_addr.sin_addr);
//			ntohl(clnt_addr.sin_addr.s_addr); // binary of 127.0.0.1

			auto clnt_port = clnt_addr.sin_port;
			std::cout << "port: " << clnt_port << std::endl;
			ProcessRequest(clnt_sock);
			CloseSocket(clnt_sock);
		}
	}

	inline void SetRouter(const Router router) {
		http_router = router;
	}

private:
	int serv_socket { };
	static constexpr int buffer_size { 1024 };
	Router http_router { };

	Sock_type StartUpSocket(const Port_type& port);
	void ProcessRequest(Sock_type clnt_sock);

	std::vector<std::string> ReadRequest(const Sock_type& clnt_sock);

	std::tuple<Request_Method, URL_type, Http_Version> ParseRequestLine(
			const std::vector<std::string>& request_data);
	//Headers ParseRequestHeader(const std::vector<std::string>& request_data);

	std::string ReadString(Sock_type sock, size_t count);
	int GetLine(Sock_type sock, char* buf, int size);
	std::string::const_iterator NextWord(std::string::const_iterator begin,
			std::string::const_iterator end);

	Request_Method GetRequestMethod(const std::string & str);
	Http_Version GetHttpVersion(const std::string & str);
	URL_type GetRequestUrl(const std::string & str);
	std::pair<std::string, std::string> GetKeyValuePair(const std::string& str);

	inline int StrCompare(const char* str1, const char* str2);
	void CloseSocket(Sock_type sock);
};

} /* namespace chttp */

#endif /* SERVER_H_ */
