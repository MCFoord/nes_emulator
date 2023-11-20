#pragma once

#include "bus.h"
#include "cpu6502.h"

class NESSystem
{
    public:
        NESSystem();
        ~NESSystem();

    private:
        Bus* bus = nullptr;
        CPU6502* CPU = nullptr;

        void tick();
        void start();
        void reset();
};