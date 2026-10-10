#pragma once
#include <iostream>

enum class Mode {
    Neutral,
    ClientSetup,
    Server,
    Client
};

enum class State {
    Files,
    Downloads,
    Transfers,
    Settings,
};

