#include "tibiapid.h"

using namespace std;
int TibiaPid::getTibiaPid(){
	if(name == "")
		name = "Tibia";

	string data;
	char buffer[10];

	FILE *stream = popen(string("pgrep " + name).c_str(), "r");
	while(fgets(buffer, 10, stream) != NULL)
		data.append(buffer);
	pclose(stream);

	return atoi(data.c_str());
}
