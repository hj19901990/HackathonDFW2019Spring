
#include "CloudClient.h"

using namespace std;

int main()
{
	CloudClient *client = new CloudClient();

	Individual indi = client->get_info(1);

	char cur_pic[640 * 480 * 3];

	cur_pic[640 * 480 * 3 - 1] = 0xAA;

	client->send_pic(640, 480, cur_pic);
	
	


	while (1);

	return 0;
}