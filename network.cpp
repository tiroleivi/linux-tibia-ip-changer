#include "network.h"

std::string Network::getIpCangerFromWebSite(){

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		return "";

	sockaddr_in sa;	
	sa.sin_family	= AF_INET;
	sa.sin_port		= htons(80);
	struct hostent *h = gethostbyname(GOOGLECODE);
	if(!h)
		return "";

	memcpy(&sa.sin_addr.s_addr, h->h_addr_list[0], 4);
	if(connect(sock, (struct sockaddr*)&sa, sizeof(struct sockaddr)) == -1){
		return "";
	}

	char Packet[] =
		"GET /p/linux-tibia-ip-changer/wiki/Changelog HTTP/1.1\r\n"
		"Host: " GOOGLECODE "\r\n"
		"\r\n";

	int rtn = 0;
	rtn = send(sock, Packet, sizeof(Packet)-1, 0);
	if(rtn == -1)
		return "";

	char buf[100];
	std::string Buffer;
	while(true){
		rtn = recv(sock, buf, 100, 0);
		Buffer += buf;
		if(Buffer.find("</body>") != std::string::npos)
			break;
	}
	
	std::string str("Actual");
    Buffer.erase(0, Buffer.find("Actual")+7);
    Buffer.erase(Buffer.find("Actual")+4, Buffer.length());
	close(sock);
	return Buffer;
}
