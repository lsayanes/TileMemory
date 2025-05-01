
#include <iostream>
#include <algorithm>
#include <Render.h>

#include <time.h>


#include "Game.h"


int main(int argc, char* argv[])
{


    sys::Render render{ 400, 800 };
    if (render.createWindows("Memory Tiles!"))
    {
        Score score{ render };

        Game tileMemory{ render, score };
        tileMemory.play();
    }

    return 0;
}

