#include <Windows.h>
#include <UIAutomation.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <comdef.h> 

#pragma comment(lib, "UIAutomationCore.lib")
#pragma comment(lib, "OleAut32.lib")
#pragma comment(lib, "Ole32.lib")

class MyEventHandler : public IUIAutomationEventHandler {
    LONG refCount;

public:
    MyEventHandler() : refCount(1) {}

    // IUnknown
    ULONG STDMETHODCALLTYPE AddRef() override {
        return InterlockedIncrement(&refCount);
    }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG val = InterlockedDecrement(&refCount);
        if (val == 0) delete this;
        return val;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (riid == __uuidof(IUnknown) || riid == __uuidof(IUIAutomationEventHandler)) {
            *ppvObject = static_cast<IUIAutomationEventHandler*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    // Event Handler
    HRESULT HandleAutomationEvent(IUIAutomationElement* sender, EVENTID eventId) override {
        BSTR name;
        if (SUCCEEDED(sender->get_CurrentName(&name))) {
            _bstr_t bstrName(name, false); 
            std::wcout << L"[Event] Invoke triggered on: " << (wchar_t*)bstrName << std::endl;
        }
        return S_OK;
    }
};

int main() {
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM" << std::endl;
        return 1;
    }

    IUIAutomation* pAutomation = nullptr;
    hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IUIAutomation), (void**)&pAutomation);
    if (FAILED(hr) || !pAutomation) {
        std::cerr << "Failed to create UIAutomation instance" << std::endl;
        CoUninitialize();
        return 1;
    }

    MyEventHandler* handler = nullptr;
    bool handlerAdded = false;

    while (!handlerAdded) {
        POINT pt;
        if (!GetCursorPos(&pt)) {
            std::cerr << "Failed to get Cursor position" << std::endl;
            break;
        }

        IUIAutomationElement* element = nullptr;
        if (SUCCEEDED(pAutomation->ElementFromPoint(pt, &element)) && element) {
            BSTR name;
            if (SUCCEEDED(element->get_CurrentName(&name))) {
                _bstr_t bstrName(name, false);
                std::wcout << L"Hovered Element Name: " << (wchar_t*)bstrName << std::endl;

                if (wcscmp((wchar_t*)bstrName, L"Start") == 0) {
                    handler = new MyEventHandler();

                    hr = pAutomation->AddAutomationEventHandler(
                        UIA_Invoke_InvokedEventId,
                        element,
                        TreeScope_Element,
                        NULL,
                        handler);

                    if (SUCCEEDED(hr)) {
                        std::wcout << L"Event handler registered on: " << (wchar_t*)bstrName << std::endl;
                        handlerAdded = true;
                    }

                    else {
                        std::wcerr << L"Failed to register event handler" << std::endl;
                        handler->Release();
                        handler = nullptr;
                    }
                }
            }
            element->Release();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::wcout << L"Listening for invoke events..." << std::endl;
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (handler && handlerAdded) {
        handler->Release();
    }

    pAutomation->Release();
    CoUninitialize();
    return 0;
}