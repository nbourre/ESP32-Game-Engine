#pragma once


struct InputConfig{
    int* inputPins = nullptr ; // Array of input pin numbers
    int count = 0;         // Number of input pins

    InputConfig(int count, ...): count(count) {
        if (count <= 0) {
            this->count = 0;
            inputPins = nullptr;
            return;
        }

        va_list args;
        va_start(args, count);

        inputPins = new int[count];
        for (int i = 0; i < count; ++i) {
            inputPins[i] = va_arg(args, int);
        }
        va_end(args);
    }
};