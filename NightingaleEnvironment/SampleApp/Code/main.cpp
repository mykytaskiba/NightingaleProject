#include "engine.h"

int main() {
    //std::thread render_thread = std::thread(render_test);
    Engine engine;
    
    engine.settings().window_title = "Sample App";
    
    engine.settings().load_commands.push_back("cpack data/load_sample_app.cpack");
    
    engine.run();
    return 0;
}