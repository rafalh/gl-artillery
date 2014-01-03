#include "stdafx.h"
#include "CApp.h"
#include "CLogger.h"

using namespace std;

int main() try
{
    srand(glfwGetTime());
    CApp App;
    App.start();
    return 0;
}
catch(exception &e)
{
    // Something went wrong
    CLogger::getInstance().error("%s\n", e.what());
    return -1;
}
