#include "tibiapid.h"

using namespace std;
int TibiaPid::getTibiaPidConsole(){
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

#ifdef __GUI__
wxArrayString TibiaPid::getTibiaPidGUI(){
	string data;
	char buffer[100];
	wxArrayString arr;

	FILE *stream = popen("pgrep Tibia", "r");
	while(fgets(buffer, 100, stream) != NULL)
		arr.Add(wxString::Format(wxT("%i"), atoi(buffer)));
	pclose(stream);

	return arr;
}
#endif
