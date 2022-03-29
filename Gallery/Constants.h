#pragma once
#include <string>
#include <vector>
#include <map>

// SQL Constants
#define USER_ID "USER_ID"
#define NAME "NAME"
#define CREATION_DATE "CREATION_DATE"
#define ID "ID"
#define LOCATION "LOCATION"

// WinAPI Constants
#define PAINT_PATH "\"mspaint\" "
#define IRFANVIEW_PATH "\"C:\\Program Files\\IrfanView\\i_view64.exe\" "
#define TERMINATE_KEY 'B'


enum CommandType
{
	HELP = 0,
	// Album operations
	CREATE_ALBUM = 1,
	OPEN_ALBUM,
	CLOSE_ALBUM,
	DELETE_ALBUM,
	LIST_ALBUMS,
	LIST_ALBUMS_OF_USER,

	// Picture operations
	ADD_PICTURE,
	REMOVE_PICTURE,
	SHOW_PICTURE,
	LIST_PICTURES,
	TAG_USER,
	UNTAG_USER,
	LIST_TAGS,

	// User operations
	ADD_USER,
	REMOVE_USER,
	LIST_OF_USER,
	USER_STATISTICS,


	TOP_TAGGED_USER,
	TOP_TAGGED_PICTURE,
	PICTURES_TAGGED_USER,

	// v1.0.3
	OPEN_PICTURE,

	// bonus
	COPY_PICTURE,
	EXIT = 99
};
enum class PhotoViewApp
{
	PAINT,
	IRFANVIEW
};

struct CommandPrompt {
	CommandType type;
	const std::string prompt;
};

struct CommandGroup {
	const std::string title;
	const std::vector<struct CommandPrompt> commands;
};