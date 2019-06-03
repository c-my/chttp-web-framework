#include <iostream>

#include "server.h"
#include "url.h"

using namespace std;

int main() {
	chttp::Server s { };
	chttp::Router router { };
	router.Get("/about", [](chttp::Request req, chttp::Response res) {
		cout << req.URL().GetURLParam("key") << endl;
		res.ServeHtml("index.html");
	});
	s.SetRouter(router);
	try {
		s.Test(1113);
	} catch (runtime_error& error) {
		cout << error.what() << endl;
		cout << "retry" << endl;
		s.Test(1114);
	}
	return 0;
}
