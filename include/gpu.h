#ifndef GPU_H
#define GPU_H

//#include "../mainwindow.h"
#include <vector>

namespace component{
    class gpu {
    private:
        char* memory;
        //char* registers;
        //bool* F;
        //unsigned short pc; // internal register

        //MainWindow* window;
    public:
        //void execute(int opcode);
        std::vector<unsigned char> getFrame();
        gpu(char*/*, MainWindow**/);
    };
}


#endif // GPU_H
