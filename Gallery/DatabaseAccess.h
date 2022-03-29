#pragma once

#include "IDataAccess.h"
#include "sqlite3.h"
#include <io.h>

#define DB_NAME "GalleryDB.db"

class DatabaseAccess : public IDataAccess
{
public:
	DatabaseAccess() = default;
	virtual ~DatabaseAccess() = default;

	// -- Stage 1 --
	bool open() override;
	void close() override;
	void clear() override;
	void closeAlbum(Album& pAlbum) override;

	// -- Stage 2 --
	void deleteAlbum(const std::string& albumName, int userId) override;
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void createUser(User& user) override;
	void deleteUser(const User& user) override;
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) override;
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) override;

	// -- Stage 3 --
	const std::list<Album> getAlbums() override;
	const std::list<Album> getAlbumsOfUser(const User& user) override;
	void createAlbum(const Album& album);
	bool doesAlbumExists(const std::string& albumName, int userId) override;
	Album openAlbum(const std::string& albumName) override;
	void printAlbums() override;

	void printUsers() override;
	User getUser(int userId) override;
	bool doesUserExists(int userId) override;
	int countAlbumsOwnedOfUser(const User& user) override;
	int countAlbumsTaggedOfUser(const User& user) override;
	int countTagsOfUser(const User& user) override;
	float averageTagsPerAlbumOfUser(const User& user) override;
	User getTopTaggedUser() override;
	Picture getTopTaggedPicture() override;
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override;

	// -- Custom --
	int lastUserId() override;
	int lastPictureId() override;

private:
	sqlite3* _db;

	bool runQuery(const std::string query);
	bool runCallbackQuery(const std::string query, const int type, void* data = nullptr);
	
	std::list<Album> _albums;
	std::list<User> _users;
};
