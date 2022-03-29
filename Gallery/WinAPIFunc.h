#pragma once
#include <iostream>
#include <windows.h>
#include "Picture.h"
#include "Constants.h"
#include <sstream>
#include <regex>

class WinAPIFunc
{
public:

	WinAPIFunc() = default;
	static void openInApp(const PhotoViewApp pva, const Picture picture);
	static std::string copyPicture(const Picture picture);
private:
	static BOOL FileExists(const LPCTSTR szPath);
};