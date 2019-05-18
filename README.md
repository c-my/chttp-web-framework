## example

~~~c++
#include <iostream>
#include "server.h"

using namespace std;

int main() {
	chttp::Server s { };
	chttp::Router router { };
	router.Get("/about", [](chttp::Request req, chttp::Response res) {
		res.ServeHtml("index.html");
	});
	s.SetRouter(router);
	try {
		s.Test(1113);
	} catch (runtime_error& error) {
		cout << error.what() << endl;
	}
	return 0;
}

~~~

