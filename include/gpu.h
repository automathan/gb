#ifndef GPU_H
#define GPU_H

//#include "../mainwindow.h"
#include <vector>

namespace component{
    class gpu {
    private:
        unsigned char* memory;
        int testoffset = 0x06;
        //char* registers;
        //bool* F;
        //unsigned short pc; // internal register

        //MainWindow* window;
    public:
        //void execute(int opcode);
        int getTO();
        std::vector<unsigned char> getFrame(int);
        gpu(unsigned char*/*, MainWindow**/);
    };
}


#endif // GPU_H
