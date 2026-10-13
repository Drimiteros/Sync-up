#include "Disk_operations.h"

using namespace std;

Disk_operations::Disk_operations() {

}

void Disk_operations::getDiskNames() {
    drives.clear();
    labels.clear();

    DWORD bufferSize = GetLogicalDriveStringsW(0, NULL);
    if (bufferSize == 0) {
        cerr << "Failed to get drive strings size. Error: " << GetLastError() << endl;
        return;
    }

    vector<wchar_t> driveStrings(bufferSize);
    if (!GetLogicalDriveStringsW(bufferSize, driveStrings.data())) {
        cerr << "Failed to get drive strings. Error: " << GetLastError() << endl;
        return;
    }

    for (auto drive = driveStrings.begin(); drive != driveStrings.end(); drive++) {
        wchar_t* drivePath = &*drive;
        wchar_t volumeName[MAX_PATH + 1] = { 0 };
        wchar_t fileSystemName[MAX_PATH + 1] = { 0 };

        BOOL success = GetVolumeInformationW(
            drivePath,
            volumeName,
            ARRAYSIZE(volumeName),
            NULL, NULL, NULL,
            fileSystemName,
            ARRAYSIZE(fileSystemName)
        );
        if (isalpha(*drive)) {
            drives.push_back(*drive);
            if (wcslen(volumeName) > 0)
                labels.push_back(volumeName);
            else
                labels.push_back(L"Local Disk");
            full_names.push_back(wstring(1, drives.back()) + L":\\" + labels.back());
            wcout << "Drives: " << drives.back() << " | " << labels.back() << endl;
        }
    }
}