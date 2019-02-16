#include "TCPClient.h"
#include "TCPServer.h"
#include "TCPSSLServer.h"
#include "TCPSSLClient.h"

#include "CloudServer.h"
#include "Protocols.h"

#include <ctime>
using namespace std;

clock_t last = clock();

int global_timestamp = 0;

void func(int timestamp, Pic *pic) {
	
	clock_t now = clock();
	double elapsed_secs = double(now - last) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;
	cout << "Frame Rate: " << 1 / elapsed_secs << endl;

	cout << pic->buffer[640*480*3-1] << endl;

	last = now;

	global_timestamp = timestamp;
}

int main() {

	CloudServer cloud(&func);

	Individual in;

	in.id = 1314;
	in.name = "Jinliang Wei";

	cloud.send_info(global_timestamp, in);

	while (1);

	return 0;
}