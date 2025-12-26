#include "pch.h"

#include "defines.h"

#include "engine.h"
#include "logger.h"
#include <thread>

#include "game_object.h"
#include "vector.h"
#include "matrix.h"


void render_test() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "render test" << std::endl;
    }
}

int main() {
    //std::thread render_thread = std::thread(render_test);
    Engine engine;
    engine.run();
    return 0;
}

