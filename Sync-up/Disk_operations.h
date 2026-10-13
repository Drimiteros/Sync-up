#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

class Disk_operations {
private:

protected:

public:
    std::vector<wchar_t> drives;
    std::vector<std::wstring> labels;
    std::vector<std::wstring> full_names;

    Disk_operations();
    void getDiskNames();
};

