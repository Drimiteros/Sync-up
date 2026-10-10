#pragma once
#include <iostream>

enum class Mode {
    Neutral,
    ClientSetup,
    ServerSetup,
    Server,
    Client
};

enum class State {
    Files,
    Downloads,
    Transfers,
    Settings,
};

