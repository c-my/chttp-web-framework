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

void Headers::SetHeader(const std::string& key, const std::string& value) {
	content[key] = value;
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

