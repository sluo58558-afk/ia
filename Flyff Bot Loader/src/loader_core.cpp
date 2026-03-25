#include "pch.h"
#include "loader_core.h"

namespace loader {

std::vector<Process> GetRunningProcessesByNames(
    const std::initializer_list<std::wstring> process_names) {
    std::vector<Process> processes;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE)
        return processes;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    
    if (Process32First(snapshot, &pe)) {
        do {
            for (auto& name : process_names) {
                if (pe.szExeFile == name) {
                    Process p;
                    p.process_name = pe.szExeFile;
                    p.process_id = pe.th32ProcessID;
                    processes.push_back(p);
                    break;
                }
            }
        } while (Process32Next(snapshot, &pe));
    }
    
    CloseHandle(snapshot);
    return processes;
}

void SuspendOrResumeProcess(const uint32_t process_id, const bool suspend) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE)
        return;

    THREADENTRY32 te;
    te.dwSize = sizeof(te);
    
    if (Thread32First(snapshot, &te)) {
        do {
            if (te.th32OwnerProcessID == process_id) {
                HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                if (thread) {
                    if (suspend)
                        SuspendThread(thread);
                    else
                        ResumeThread(thread);
                    CloseHandle(thread);
                }
            }
        } while (Thread32Next(snapshot, &te));
    }
    
    CloseHandle(snapshot);
}

void Inject(const uint32_t process_id) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    
    if (process) {
        wchar_t dll_path[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, dll_path);
        wcscat_s(dll_path, L"\\RIMJOB.module.dll");
        
        LPVOID load_lib = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
        LPVOID remote_mem = VirtualAllocEx(process, NULL, (wcslen(dll_path) + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
        
        if (remote_mem) {
            WriteProcessMemory(process, remote_mem, dll_path, (wcslen(dll_path) + 1) * sizeof(wchar_t), NULL);
            HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)load_lib, remote_mem, 0, NULL);
            
            if (thread) {
                WaitForSingleObject(thread, INFINITE);
                VirtualFreeEx(process, remote_mem, 0, MEM_RELEASE);
                CloseHandle(thread);
            }
        }
        
        CloseHandle(process);
    }
}

void InjectHwidBypass(const uint32_t process_id) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    
    if (process) {
        wchar_t dll_path[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, dll_path);
        wcscat_s(dll_path, L"\\HWID Bypasser.dll");
        
        LPVOID load_lib = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
        LPVOID remote_mem = VirtualAllocEx(process, NULL, (wcslen(dll_path) + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
        
        if (remote_mem) {
            WriteProcessMemory(process, remote_mem, dll_path, (wcslen(dll_path) + 1) * sizeof(wchar_t), NULL);
            HANDLE thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)load_lib, remote_mem, 0, NULL);
            
            if (thread) {
                WaitForSingleObject(thread, INFINITE);
                VirtualFreeEx(process, remote_mem, 0, MEM_RELEASE);
                CloseHandle(thread);
            }
        }
        
        CloseHandle(process);
    }
}

}  // namespace loader
