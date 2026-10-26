
# ProcWatch

monitors running processes and sorts them by memory usage

---

enumerates running Windows processes, retrieves their working-set memory usage when accessible, and displays the top 10 processes by memory consumption.

## features

- enumerates running processes using the Windows Tool Help API
- retrieves process memory statistics through the Windows API
- sorts processes by working-set memory usage, highest first
- displays process names, PIDs, and memory usage in MB
- handles processes that cannot be opened or queried

## requirements

- Windows
- Visual Studio + Windows SDK
- Windows SDK headers and Psapi.lib

## build

Open the project in Visual Studio and build for your target configuration.

The project links against `Psapi.lib` for `GetProcessMemoryInfo`.

## usage

Run the compiled executable:

```text
ProcWatch.exe
```

## output

```text
=== ProcWatch | Display your apps with most usage ===

Process                            PID         Memory (MB)
---------------------------------------------------------------
chrome.exe                         4820        624.50
Discord.exe                        7364        412.25
explorer.exe                       1920        85.20
...

Processes displayed: 20
```

Example output only.

## notes

- Displays at most 20 processes.
- Memory usage represents the process working set, not total virtual memory.
- Some processes may be omitted if Windows denies access or memory information cannot be retrieved.
