/*
 * socket.h
 *
 *  Created on: June 2, 2019
 *      Author: cmy
 */

#include "url.h"
using namespace std;
std::string chttp::URL::GetURLParam(const std::string& key)const
{
	smatch result;
	if (regex_search(_url.cbegin(), _url.cend(), result, regex{ key + "=(.*?)&" })
		|| regex_search(_url.cbegin(), _url.cend(), result, regex{ key + "=(.*)" })) {
		return result[1];
	}
	else {
		return string{};
	}
}
