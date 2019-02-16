#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"

#include "CloudServer.h"
#include "Protocols.h"

#include <ctime>
using namespace std;

clock_t last = clock();

void func(Pic *pic) {
	
	clock_t now = clock();
	double elapsed_secs = double(now - last) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;
	cout << "Frame Rate: " << 1 / elapsed_secs << endl;

	cout << pic->buffer[640*480*3-1] << endl;

	last = now;
}

int main() {

	CloudServer cloud(&func);

	

	while (1);

	return 0;
}