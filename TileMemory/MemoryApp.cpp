

#include <iostream>
#include "Render.h"
#include "Entity.h"

int main(int argc, char* argv[])
{
    Render render{ 400, 600 };

    if(render.createWindows("Memory Tiles!"))
    {
        auto red = render.addEntity("./Img/red.png");
        auto blue = render.addEntity("./Img/blue.png");

        red.x = 0;
        blue.x = 50;

        render.doEvent();
    }

    return 0;
}

