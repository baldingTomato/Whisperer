#pragma once
#include <windows.h>

class WindowsApi {
public:
    virtual ~WindowsApi() = default;

    virtual bool registerHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk) const = 0;
    virtual bool unregisterHotKey(HWND hWnd, int id) const = 0;
    virtual UINT sendInput(UINT cInputs, LPINPUT pInputs, int cbSize) const = 0;
    virtual bool getMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) const = 0;
};
