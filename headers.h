/*
 * headers.h
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>

namespace chttp {

class Headers final {
public:
	constexpr static char PlainText[] = "text/plain";
	constexpr static char HtmlText[] = "text/html";
	constexpr static char CSSText[] = "text/css";
	constexpr static char JavascriptText[] = "textjavascript";
	constexpr static char GifImage[] = "image/gif";
	constexpr static char PngImage[] = "image/png";
	constexpr static char JpegImage[] = "image/jpeg";
	constexpr static char BmpImage[] = "image/bmp";
	constexpr static char WebpImage[] = "image/webp";
	constexpr static char MidiAudio[] = "audio/midi";
	constexpr static char MpegAudio[] = "audio/mpeg";
	constexpr static char WebmAudio[] = "audio/webm";
	constexpr static char OggAudio[] = "audio/ogg";
	constexpr static char WavAudio[] = "audio/wav";
	constexpr static char WebmVideo[] = "video/webm";
	constexpr static char OggVideo[] = "video/ogg";
	constexpr static char XmlApplication[] = "application/xml";
	constexpr static char PDFApplication[] = "application/pdf";

	Headers();
	virtual ~Headers();

	void SetHeader(std::string& key, const std::string& value);

	inline const std::unordered_map<std::string, std::string>& Content() const {
		return content;
	}

	inline std::unordered_map<std::string, std::string>& Content() {
		return content;
	}

	int ContentLength() const;

	friend std::ostream& operator<<(std::ostream& out, const Headers &h);

private:
	inline std::string GetValue(std::string key) const;

	std::unordered_map<std::string, std::string> content { };
};

} /* namespace chttp */

#endif /* HEADERS_H_ */
