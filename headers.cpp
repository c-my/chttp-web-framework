/*
 * headers.cpp
 *
 *  Created on: May 16, 2019
 *      Author: cmy
 */

#include "headers.h"

using namespace std;
namespace chttp {

Headers::Headers() {
	// TODO Auto-generated constructor stub

}

Headers::~Headers() {
	// TODO Auto-generated destructor stub
}

void Headers::SetHeader(std::string& key, const std::string& value) {
	transform(key.begin(), key.end(), key.begin(), ::tolower);
	content[key] = value;
}

int Headers::ContentLength() const
{
	string len = GetValue("Content-Length");
	if (len.empty())return 0;
	return stoi(len);
}

inline std::string Headers::GetValue(std::string key) const
{
	transform(key.begin(), key.end(), key.begin(), ::tolower);
	if (content.count(key) == 0)
		return std::string();
	return content.at(key);
}

ostream& operator<<(ostream& out, const chttp::Headers &h) {
	for (auto &p : h.content) {
		out << p.first;
		out << ": ";
		out << p.second;
		out << '\n';
	}
	return out;
}

} /* namespace chttp */

