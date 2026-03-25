#include <Windows.h>
#include <commctrl.h>
#include "resource.h"
#include <string>
#include <vector>
#include <cwchar>

#pragma comment(lib, "comctl32.lib")

struct ProcessInfo {
    std::wstring process_name;
    DWORD process_id;
    std::wstring window_name;
};

std::vector<ProcessInfo> GetProcesses() {
    std::vector<ProcessInfo> processes;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE)
        return processes;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    
    if (Process32First(snapshot, &pe)) {
        do {
            ProcessInfo pi;
            pi.process_name = pe.szExeFile;
            pi.process_id = pe.th32ProcessID;
            
            if (pi.process_name == L"Kingdom FlyFF Bot.exe" ||
                pi.process_name == L"neuz_patched.exe" ||
                pi.process_name == L"Neuz_patched.exe" ||
                pi.process_name == L"Neuz.exe" ||
                pi.process_name == L"Code Tester.exe" ||
                pi.process_name == L"data.dll") {
                
                processes.push_back(pi);
            }
        } while (Process32Next(snapshot, &pe));
    }
    
    CloseHandle(snapshot);
    return processes;
}

void InjectDLL(DWORD process_id, const wchar_t* dll_path) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    
    if (process) {
        LPVOID load_lib = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
        LPVOID remote_mem = VirtualAllocEx(process, NULL, (wcslen(dll_path) + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        
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

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hListView;
    
    switch (msg) {
        case WM_INITDIALOG: {
            hListView = GetDlgItem(hwnd, LIST_PROCESSES);
            
            INITCOMMONCONTROLSEX icc = {0};
            icc.dwSize = sizeof(icc);
            icc.dwICC = ICC_LISTVIEW_CLASSES;
            InitCommonControlsEx(&icc);
            
            LVCOLUMN lvc = {0};
            lvc.mask = LVCF_TEXT | LVCF_WIDTH;
            lvc.pszText = L"Process Name";
            lvc.cx = 150;
            ListView_InsertColumn(hListView, 0, &lvc);
            
            lvc.pszText = L"Process ID";
            lvc.cx = 80;
            ListView_InsertColumn(hListView, 1, &lvc);
            
            lvc.pszText = L"Window";
            lvc.cx = 100;
            ListView_InsertColumn(hListView, 2, &lvc);
            
            auto processes = GetProcesses();
            
            for (size_t i = 0; i < processes.size(); ++i) {
                LVITEM lvi = {0};
                lvi.mask = LVIF_TEXT;
                lvi.iItem = (int)i;
                lvi.pszText = (LPWSTR)processes[i].process_name.c_str();
                int idx = ListView_InsertItem(hListView, &lvi);
                
                ListView_SetItemText(hListView, idx, 1, 
                    (LPWSTR)std::to_wstring(processes[i].process_id).c_str());
                ListView_SetItemText(hListView, idx, 2, L"");
            }
            
            return TRUE;
        }
        
        case WM_COMMAND:
            if (LOWORD(wParam) == BUTTON_INJECT) {
                int sel = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
                if (sel != -1) {
                    wchar_t buf[64];
                    ListView_GetItemText(hListView, sel, 1, buf, 64);
                    DWORD pid = wcstoul(buf, NULL, 10);
                    
                    wchar_t dll_path[MAX_PATH];
                    GetCurrentDirectory(MAX_PATH, dll_path);
                    wcscat_s(dll_path, L"\\RIMJOB.module.dll");
                    
                    InjectDLL(pid, dll_path);
                    EndDialog(hwnd, 0);
                    PostQuitMessage(0);
                }
            } else if (LOWORD(wParam) == BUTTON_BYPASS_CLIENT_LIMIT) {
                MessageBox(hwnd, L"Bypass feature not fully implemented in this simplified version", L"Info", MB_OK);
            } else if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hwnd, 0);
                PostQuitMessage(0);
            }
            break;
    }
    
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_MAIN), NULL, DialogProc);
    return 0;
}
