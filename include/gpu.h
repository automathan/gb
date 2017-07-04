#ifndef GPU_H
#define GPU_H

//#include "../mainwindow.h"
#include <vector>

namespace component{
    class gpu {
    private:
        unsigned char* memory;
        //char* registers;
        //bool* F;
        //unsigned short pc; // internal register

        //MainWindow* window;
    public:
        //void execute(int opcode);
        std::vector<unsigned char> getFrame();
        gpu(unsigned char*/*, MainWindow**/);
    };
}


#endif // GPU_H
