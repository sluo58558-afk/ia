#pragma once
#include <Windows.h>

class ProcessSelectorWindow {
public:
    virtual void Initialize(const HWND window_handle);
    virtual void OnButtonClick(const HWND button_handle, const uint32_t button_id);
    virtual void OnListViewDoubleClick(NMITEMACTIVATE* item);
};
