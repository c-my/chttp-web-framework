/*
 * socket.h
 *
 *  Created on: June 2, 2019
 *      Author: cmy
 */

#ifndef URL_H_
#define URL_H_

#include <string>
#include <regex>
#include <iostream>

namespace chttp {
class URL
{
public:
	URL(const std::string url) :_url(url) {}
	URL(const char* url) :_url(std::string{ url }) {}

	inline std::string GetURL() const { return _url; }
	std::string GetParam(const std::string& key);

	inline bool operator == (const URL& other)const { return this->_url == other._url; }
	inline bool operator != (const URL& other)const { return this->_url != other._url; }

	friend std::ostream& operator <<(std::ostream& out, const URL& url) {
		out << url._url;
		return out;
	}

private:
	std::string _url;
};
}

#endif