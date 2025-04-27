
#include <Windows.h>

#include <iostream>
#include <algorithm>
#include <Render.h>

#include <time.h>


#include "Game.h"

uint64_t score = 0;
uint8_t level = 0;
uint8_t stages = 1;


int main(int argc, char* argv[])
{

    sys::Render render{ 400, 800 };
    if (render.createWindows("Memory Tiles!"))
    {
        Game tileMemory{ render };
        tileMemory.play();
    }

}

