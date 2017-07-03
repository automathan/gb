#include "cpu.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <QObject>
/*
    The emulator class can be seen as the
    actual Game Boy unit, consisting of
    different circuits/components that
    have different responsibilities
*/

class emulator : public QObject{
    Q_OBJECT
private:
    char* memory;
    char* registers;
    bool* F;

    struct ROM{
    	std::string name;
    	int size;
    	char* cartridge;
    };

    component::cpu* cpu;
    ROM* loadROM(const char*); // TODO move to romloader
public:
    void play(const char*);
signals:
    void debug(const char*);
};
