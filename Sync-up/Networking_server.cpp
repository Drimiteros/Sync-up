#include "Networking_server.h"

using namespace std;
using namespace sf;

Networking_server::Networking_server() {

}

bool Networking_server::run() {
	if (listener.listen(port) != Socket::Done) {
		cout << "Failed to bind server on port: " << port << endl;
		return false;
	}

	selector.add(listener);
	isRunning = true;
	cout << "Server started on port: " << port << endl;

	while (isRunning) {
		if (selector.wait(milliseconds(100))) {
			if (selector.isReady(listener)) {
				
			}
			else {

			}
		}
	}
}

void Networking_server::stop() {
	isRunning = false;
}