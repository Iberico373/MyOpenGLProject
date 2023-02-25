#include "Application.h"
#include <iostream>

int main()
{
	Application application;

	if (application.Init() == true)
	{
		application.Run();
	}

	return 0;
}
