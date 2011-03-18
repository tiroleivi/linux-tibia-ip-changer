#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#ifdef __GUI__
#include <wx/arrstr.h>
#include <wx/string.h>
#endif

class TibiaPid
{
public:
	std::string name;
    int getTibiaPidConsole();
#ifdef __GUI__
	wxArrayString getTibiaPidGUI();
#endif
};
