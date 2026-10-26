
#pragma comment(lib, "Psapi.lib")

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>
#include <vector>
#include <algorithm>
#include <iomanip>

struct ProcessInfo {
    std::wstring name;
    DWORD pid;
    double memoryMB;
};

int main() {
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0
    );

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "[!] Failed to get snapshot.\n";
        return 1;
    }

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    std::vector<ProcessInfo> processes;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            HANDLE process = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE,
                entry.th32ProcessID
            );

            if (process != nullptr) {
                PROCESS_MEMORY_COUNTERS memoryInfo{};
                memoryInfo.cb = sizeof(memoryInfo);

                if (GetProcessMemoryInfo(
                    process,
                    &memoryInfo,
                    sizeof(memoryInfo)
                )) {
                    double memoryMB =
                        static_cast<double>(memoryInfo.WorkingSetSize)
                        / (1024.0 * 1024.0);

                    processes.push_back(ProcessInfo{
                        entry.szExeFile,
                        entry.th32ProcessID,
                        memoryMB
                        });
                }

                CloseHandle(process);
            }

        } while (Process32NextW(snapshot, &entry));
    }
    else {
        std::cout << "[!] Failed to retrieve the first process.\n";
    }

    CloseHandle(snapshot);

    std::sort(
        processes.begin(),
        processes.end(),
        [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.memoryMB > b.memoryMB;
        }
    );

    std::wcout << L"\n=== ProcWatch | Display your apps with most usage ===\n\n";

    std::wcout << std::left
        << std::setw(35) << L"Process"
        << std::setw(12) << L"PID"
        << L"Memory (MB)\n";

    std::wstring border(25,'-');
    std::wcout << border << "\n";

    const size_t limit = (std::min)(processes.size(), static_cast<size_t>(20));

    for (size_t i = 0; i < limit; ++i) {
        std::wcout << std::left
            << std::setw(35) << processes[i].name
            << std::setw(12) << processes[i].pid
            << std::fixed << std::setprecision(2)
            << processes[i].memoryMB << L"\n";
    }

    std::wcout << L"\nProcesses displayed: " << limit << L"\n";

    return 0;
}
