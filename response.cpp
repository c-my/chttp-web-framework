/*
 * response.cpp
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#include "response.h"

using namespace std;

namespace chttp {
constexpr char Headers::PlainText[];
constexpr char Headers::HtmlText[];
constexpr char Headers::CSSText[];
constexpr char Headers::JavascriptText[];
constexpr char Headers::GifImage[];
constexpr char Headers::PngImage[];
constexpr char Headers::JpegImage[];
constexpr char Headers::BmpImage[];
constexpr char Headers::WebpImage[];
constexpr char Headers::MidiAudio[];
constexpr char Headers::MpegAudio[];
constexpr char Headers::WebmAudio[];
constexpr char Headers::OggAudio[];
constexpr char Headers::WavAudio[];
constexpr char Headers::WebmVideo[];
constexpr char Headers::OggVideo[];
constexpr char Headers::XmlApplication[];
constexpr char Headers::PDFApplication[];

constexpr char HttpStatus::OK[];
constexpr char HttpStatus::BadRequest[];
constexpr char HttpStatus::Unauthorized[];
constexpr char HttpStatus::Forbidden[];
constexpr char HttpStatus::NotFound[];
constexpr char HttpStatus::Unavailable[];

constexpr char HttpVersionStr::Http1_0[];
constexpr char HttpVersionStr::Http1_1[];
constexpr char HttpVersionStr::Http2_0[];

string Response::NotFoundPage = "not_found.html";

Response::~Response() {
	// TODO Auto-generated destructor stub
}

void Response::ServeHtml(const char* file_path) {
	SendStatusLine(HttpVersionStr::Http1_1, HttpStatus::OK);
	SetContentType(Headers::HtmlText);
	SendHeader(header);

	SendFile(clnt_sock, file_path);
	CloseSocket(clnt_sock);
}

void Response::SendFile(Sock_type clnt_sock, const char* file_path, bool show_not_found) {
//#ifdef __linux__
	FILE* file = fopen(file_path, "r");
	if (file == NULL) {
		if (show_not_found)
			return NotFound();
		else
			return;
	} else {
		char buffer[buffer_size];
		fgets(buffer, sizeof(buffer), file);
		while (!feof(file)) {
			WriteSocket(clnt_sock, buffer, strlen(buffer));
			fgets(buffer, sizeof(buffer), file);
		}
	}
	fclose(file);
//#endif
}
void Response::SendHeader(const Headers& headers) {
	const auto content = headers.Content();
	for (auto & header_pair : content) {
		WriteSocket(clnt_sock, header_pair.first.c_str(), 0);
		WriteSocket(clnt_sock, ": ", 2);
		WriteSocket(clnt_sock, header_pair.second.c_str(), 0);
		WriteSocket(clnt_sock, "\r\n", 0);
	}
	WriteSocket(clnt_sock, "\r\n", 0);
}

void Response::SendStatusLine(const char* http_version,
		const char* http_status) {
	WriteSocket(clnt_sock, http_version);
	WriteSocket(clnt_sock, " ");
	WriteSocket(clnt_sock, http_status);
	WriteSocket(clnt_sock, "\r\n");
}

void Response::NotFound() {
	SendStatusLine(HttpVersionStr::Http1_1, HttpStatus::NotFound);
	SetContentType(Headers::HtmlText);
	SendHeader(header);
	SendFile(clnt_sock, NotFoundPage.c_str(), false);
}

void Response::WriteSocket(Sock_type sock, const char* buffer, size_t size,
		int flags) {
	int length = static_cast<int>(size);
	if (size == 0)
		length = static_cast<int>(strlen(buffer));
	send(sock, buffer, length, flags);
#ifdef DEBUG
	std::cout << "write to socket:";
	for (size_t i = 0; i < length; ++i)
		cout << buffer[i];
#endif // DEBUG
}

inline void Response::CloseSocket(Sock_type sock)
{
#ifdef __linux__
	return close(sock);
#elif _MSC_VER
	closesocket(sock);
	return;
#endif
}

} /* namespace chttp */

