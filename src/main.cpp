#include "stdafx.h"
#include "CApp.h"

using namespace std;

int main()
{
    srand(glfwGetTime());
    CApp App;
    App.start();
    return 0;
}
