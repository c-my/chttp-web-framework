/*
 * socket.cpp
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#include "server.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::runtime_error;

namespace chttp {

Server::Server() {
	// TODO Auto-generated constructor stub

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

int Server::GetLine(Sock_type sock, char* buf, int size) {
//#ifdef __linux__
	int count { };
	char c = '\0';
	int n { };
	while ((count < size - 1) && (c != '\n')) {
		n = recv(sock, &c, 1, 0);
		if (n > 0) {
			if (c == '\r') {
				n = recv(sock, &c, 1, MSG_PEEK); //peek the next char
				if ((n > 0) && (c == '\n')) {
					recv(sock, &c, 1, 0);
					break;
				} else
					c = '\n';
			}
			buf[count] = c;
			count++;
		} else {
			c = '\n';
		}
	}
	buf[count] = '\0';
	return count;

//#endif
}

vector<string> Server::ReadRequest(const Sock_type& clnt_sock) {
	int char_count { 1 };
	char buffer[buffer_size];
	vector<string> content { };
	while (char_count > 0 && strcmp("\n", buffer)) {
		char_count = GetLine(clnt_sock, buffer, sizeof(buffer));
		content.push_back(buffer);
	}
	return content;
}

Server::Sock_type Server::StartUpSocket(const Port_type& port) {
#ifdef __linux__
	int serv_sock { };
	sockaddr_in serv_addr { };

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		throw std::runtime_error { "socket_create_error" };
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(serv_sock, (sockaddr*) (&serv_addr), sizeof(serv_addr)) < 0) {
		throw std::runtime_error { "socket_bind_error" };
	}

	if (listen(serv_sock, 5) == -1) {
		throw std::runtime_error { "socket_listen_error" };
	}
	return serv_sock;
#endif __linux__ //__linux__

#ifdef _MSC_VER
	WSADATA wsaData{};
	SOCKET serv_sock{};
	SOCKADDR_IN serv_addr;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		throw std::runtime_error{ "win_socket_startup_error" };
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == INVALID_SOCKET) {
		throw std::runtime_error{ "socket_create_error" };
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if (bind(serv_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		throw std::runtime_error{ "socket_bind_error" };
	}

	if (listen(serv_sock, 5) == SOCKET_ERROR) {
		throw std::runtime_error{ "socket_listen_error" };
	}

	return serv_sock;
#endif // _MSC_VER
}

void Server::ProcessRequest(Sock_type clnt_sock) {
	auto request_string = ReadRequest(clnt_sock);
	auto request_line = ParseRequestLine(request_string);
	Request_Method method = std::get<0>(request_line);
	URL_type url = URL{ std::get<1>(request_line) };
	Http_Version version = std::get<2>(request_line);
	Headers header { };
	for (auto it = request_string.begin() + 1; it + 1 != request_string.end(); // ignore the last empty line
			++it) {
		auto pair = GetKeyValuePair(*it);
		header.SetHeader(pair.first, pair.second);
	}
	cout << "url: " << url << endl;
	cout << "Headers:\n" << header << endl;
	// TODO add client info to request
	Request request { method, url, version, header };
	Response response { clnt_sock };
	
	RequestInfo request_info { url, method };
	if (http_router.Contains(request_info)) {
		auto handler = http_router.GetHandler(request_info);
		handler(request, response);
		cout << "find existing routers" << endl;
	} else {
		cout << "not find existing routers" << endl;
		response.NotFound();
	}
}

std::string::const_iterator Server::NextWord(std::string::const_iterator begin,
		std::string::const_iterator end) {
	for (auto it = begin; it != end; ++it) {
		if (*it == ' ' || *it == '\0') {
			return it;
		}
	}
	return end;
}

std::tuple<Request_Method, URL_type, Http_Version> Server::ParseRequestLine(
		const std::vector<std::string>& request_data) {
	auto method_string_end = NextWord(request_data[0].cbegin(),
			request_data[0].cend());
	auto url_string_end = NextWord(method_string_end + 1,
			request_data[0].end());
	auto method_string = string { request_data[0].cbegin(), method_string_end };
	auto url_string = string { method_string_end + 1, url_string_end };
	auto version_string_end = NextWord(url_string_end + 1,
			request_data[0].end());
	auto version_string = string { url_string_end + 1, version_string_end };

	auto method = GetRequestMethod(method_string);
	auto url = GetRequestUrl(url_string);
	auto http_version = GetHttpVersion(version_string.c_str());

	return std::make_tuple(method, url, http_version);
}

Request_Method Server::GetRequestMethod(const std::string& str) {
	auto c_str = str.c_str();
	if (StrCompare(c_str, "get") == 0) {
		return Request_Method::GET;
	} else if (StrCompare(c_str, "post") == 0) {
		return Request_Method::POST;
	} else if (StrCompare(c_str, "head") == 0) {
		return Request_Method::HEAD;
	} else if (StrCompare(c_str, "put") == 0) {
		return Request_Method::PUT;
	} else if (StrCompare(c_str, "delete") == 0) {
		return Request_Method::DELETE_M;
	} else if (StrCompare(c_str, "options") == 0) {
		return Request_Method::OPTIONS;
	} else if (StrCompare(c_str, "connect") == 0) {
		return Request_Method::CONNECT;
	} else if (StrCompare(c_str, "trace") == 0) {
		return Request_Method::TRACE;
	} else {
		return Request_Method::NONE;
	}
}

Http_Version Server::GetHttpVersion(const std::string& str) {
	auto c_str = str.c_str();
	if (StrCompare(c_str, "HTTP/1.1")) {
		return Http_Version::HTTP1_1;
	} else if (StrCompare(c_str, "HTTP/1.0")) {
		return Http_Version::HTTP1_0;
	} else if (StrCompare(c_str, "HTTP/2.0")) {
		return Http_Version::HTTP2_0;
	} else {
		return Http_Version::NONE;
	}
}

URL_type Server::GetRequestUrl(const std::string& str) {
	return URL_type { str };
}

std::pair<std::string, std::string> Server::GetKeyValuePair(
		const std::string& str) {
	auto it = str.find(": ");
	if (it == string::npos) {
		throw runtime_error { "no colon-blank separator found" };
	}
	string key { str.cbegin(), str.begin() + it };
	string value { str.begin() + it + 2, str.end() };
	return make_pair(key, value);
}

inline int Server::StrCompare(const char* str1, const char* str2)
{
#ifdef __linux__
	return strcasecmp(str1, str2);
#endif
#ifdef _MSC_VER
	return _stricmp(str1, str2);
#endif
}

void Server::CloseSocket(Sock_type sock)
{
#ifdef __linux__
	return close(sock);
#elif _MSC_VER
	closesocket(sock);
	return;
#endif
}

} /* namespace chttp */

