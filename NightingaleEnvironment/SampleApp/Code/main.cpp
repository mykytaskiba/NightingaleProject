#include "engine.h"
#include <iostream>

int main() {

    //std::thread render_thread = std::thread(render_test);
    Engine engine;
    
    engine.settings().load_commands.push_back("load_sample_app");
    
    engine.run();
    return 0;
}
