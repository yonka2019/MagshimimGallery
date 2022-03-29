#pragma once
#include "Picture.h"
#include <list>
#include <memory>


class Album
{
public:
    Album() = default;
	Album(int ownerId, const std::string& name);
	Album(int ownerId, const std::string& name, const std::string& creationTime);

	const std::string& getName() const;
	void setName(const std::string& name);

	int getOwnerId() const;
	void setOwner(int userId);

	std::string getCreationDate() const;
	void setCreationDate(const std::string& creationTime);
	void setCreationDateNow();

	bool doesPictureExists(const std::string& name) const;
	void addPicture(const Picture& picture);
	void removePicture(const std::string& pictureName);

	Picture getPicture(const std::string& name) const;
	std::list<Picture> getPictures() const;

	void untagUserInAlbum(int userId);
	void tagUserInAlbum(int userId);

	void untagUserInPicture(int userId, const std::string& pictureName);
	void tagUserInPicture(int userId, const std::string& pictureName);
	
	bool operator==(const Album& other) const;
	friend std::ostream& operator<<(std::ostream& strOut, const Album& album);

private:
    int m_ownerId { 0 };
	std::string m_name;
	std::string m_creationDate;
	std::list<Picture> m_pictures;
};
