#include "WinAPIFunc.h"

PROCESS_INFORMATION pi = { 0 };
HHOOK hook;

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0)
    {
        std::cout << "Application stopped" << std::endl;
        PostQuitMessage(0);
    }

    if (code == HC_ACTION)
    {
        PKBDLLHOOKSTRUCT pkb = (PKBDLLHOOKSTRUCT)lParam;
        if (wParam == WM_KEYDOWN && pkb->vkCode == TERMINATE_KEY)
        {
            if (GetAsyncKeyState(VK_CONTROL) < 0)
            {
                DWORD pid = 0;
                HWND hwnd = GetForegroundWindow();
                GetWindowThreadProcessId(hwnd, &pid);
                if (pid == pi.dwProcessId)
                {
                    TerminateProcess(pi.hProcess, 0);
                    PostQuitMessage(0);
                }
            }
        }
    }
    return CallNextHookEx(hook, code, wParam, lParam);
}

void WinAPIFunc::openInApp(const PhotoViewApp pva, const Picture picture)
{
    std::string appPath;
    STARTUPINFO si = { 0 };
    switch (pva)
    {
        case PhotoViewApp::PAINT:
            appPath = PAINT_PATH;
            break;
        case PhotoViewApp::IRFANVIEW:
            appPath = IRFANVIEW_PATH;
            break;
    }

    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = true;

    appPath.append(picture.getPath()); // + add photo path argument

    CreateProcess(NULL, const_cast<char*>(appPath.c_str()), NULL, NULL, false, 0, NULL, NULL, &si, &pi);

    if (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT)
    {
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    else
        std::cerr << "[ERROR] Something went wrong" << std::endl;

    WaitForSingleObject(pi.hProcess, -1);
    UnhookWindowsHookEx(hook);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}
std::string WinAPIFunc::copyPicture(const Picture picture)
{
    std::regex fileName(R"((.+?)(\w+)(\..+)$)");
    std::string duplicateFilePath = std::regex_replace(picture.getPath(), fileName, "$1CopyOf_$2$3"); // add Copy_Of to the beggining of the file name

    try
    {
        BOOL exists = FileExists(duplicateFilePath.c_str());
        if (exists)
            throw std::exception("File already exists.");

        BOOL copied = CopyFile(picture.getPath().c_str(), duplicateFilePath.c_str(), FALSE);
        if (!copied)
            throw std::exception("Something went wrong.");
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return "NULL";
    }

    return duplicateFilePath;
}

BOOL WinAPIFunc::FileExists(const LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
