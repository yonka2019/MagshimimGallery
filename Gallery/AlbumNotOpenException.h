#pragma once
#include "MyException.h"

class AlbumNotOpenException : public MyException {
public:
	AlbumNotOpenException(): MyException("Error: Can't perform operation, you need to open an album first") {}
};
