#include "engine.h"
#include <iostream>




    template<typename Arg>
    void func_impl(Arg& arg) {
        std::cout << "Generic ";
    }

    template<>
    void func_impl(float& arg) {
        std::cout << "Float ";
        arg = 1.0f;
    }

    template<>
    void func_impl(int& arg) {
        std::cout << "Int ";
        arg = 0;
    }

    void read_args() {
        std::cout << "End" << std::endl;
    }

    template<typename Arg, typename... Rest>
    void read_args(Arg& arg, Rest&... args) {
        func_impl(arg);
        read_args(args...);
        //(func_impl(args),0)...;
    }


int main() {

    float f1;
    float f2;
    int i1;
    read_args(f1, f2, i1);
    
    //std::thread render_thread = std::thread(render_test);
    Engine engine;
    
    engine.settings().window_title = "Sample App";
    
    engine.settings().load_commands.push_back("cpack data/load_sample_app.cpack");
    
    //engine.run();
    return 0;
}
