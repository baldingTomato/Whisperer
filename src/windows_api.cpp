#include "windows_api.hpp"

class WindowsApiImpl : public WindowsApi {
public:
    bool registerHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk) const override {
        return ::RegisterHotKey(hWnd, id, fsModifiers, vk);
    }

    bool unregisterHotKey(HWND hWnd, int id) const override {
        return ::UnregisterHotKey(hWnd, id);
    }

    UINT sendInput(UINT cInputs, LPINPUT pInputs, int cbSize) const override {
        return ::SendInput(cInputs, pInputs, cbSize);
    }

    bool getMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) const override {
        return ::GetMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    }
};
