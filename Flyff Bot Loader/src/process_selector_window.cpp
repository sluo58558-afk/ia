#include "pch.h"
#include "process_selector_window.h"
#include "loader_core.h"
#include "resource.h"

void ProcessSelectorWindow::Initialize(const HWND window_handle) {
    SetWindowText(window_handle, L"RIMJOB");
    ShowWindow(window_handle, SW_SHOW);
}

void ProcessSelectorWindow::OnButtonClick(const HWND button_handle, const uint32_t button_id) {
    switch (button_id) {
        case BUTTON_INJECT: {
            break;
        }
        case BUTTON_BYPASS_CLIENT_LIMIT: {
            break;
        }
    }
}

void ProcessSelectorWindow::OnListViewDoubleClick(NMITEMACTIVATE* item) {
    if (item->iItem != -1) {
    }
}
