#pragma once
#include <list>
#include "Album.h"
#include "User.h"

class IDataAccess
{
public:
	virtual ~IDataAccess() = default;

	// album related
	virtual const std::list<Album> getAlbums() = 0;
	virtual const std::list<Album> getAlbumsOfUser(const User& user) = 0;
	virtual void createAlbum(const Album& album) = 0;
	virtual void deleteAlbum(const std::string& albumName, int userId) = 0;
	virtual bool doesAlbumExists(const std::string& albumName, int userId) = 0;
	virtual Album openAlbum(const std::string& albumName) = 0;
	virtual void closeAlbum(Album& pAlbum) = 0;
	virtual void printAlbums() = 0;

    // picture related
	virtual void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) = 0;
	virtual void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) = 0;
	virtual void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) = 0;
	virtual void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) = 0;

	// user related
	virtual void printUsers() =0;
	virtual User getUser(int userId) = 0;
	virtual void createUser(User& user ) = 0;
	virtual void deleteUser(const User& user) = 0;
	virtual bool doesUserExists(int userId) = 0 ;
	virtual int lastUserId() = 0;
	virtual int lastPictureId() = 0;
	

	// user statistics
	virtual int countAlbumsOwnedOfUser(const User& user) = 0;
	virtual int countAlbumsTaggedOfUser(const User& user) = 0;
	virtual int countTagsOfUser(const User& user) = 0;
	virtual float averageTagsPerAlbumOfUser(const User& user) = 0;

	// queries
	virtual User getTopTaggedUser() = 0;
	virtual Picture getTopTaggedPicture() = 0;
	virtual std::list<Picture> getTaggedPicturesOfUser(const User& user) = 0;
	
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual void clear() = 0;
};
