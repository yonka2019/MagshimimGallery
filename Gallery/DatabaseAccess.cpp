#include "DatabaseAccess.h"
#include "Constants.h"

#pragma region callBackFunctions

int callbackAlbum(void* _data, int argc, char** argv, char** azColName)
{
	auto& albumsList = *static_cast<std::list<Album>*>(_data);
	Album newAlbum(0, "");

	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == USER_ID)
			newAlbum.setOwner(atoi(argv[i]));
		else if (std::string(azColName[i]) == NAME)
			newAlbum.setName(argv[i]);
		else if (std::string(azColName[i]) == CREATION_DATE)
			newAlbum.setCreationDate(argv[i]);
	}

	albumsList.push_back(newAlbum);
	return 0;
}

int callbackUser(void* _data, int argc, char** argv, char** azColName)
{
	auto& usersList = *static_cast<std::list<User>*>(_data);
	User newUser(0, "");

	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == ID)
			newUser.setId(atoi(argv[i]));
		else if (std::string(azColName[i]) == NAME)
			newUser.setName(argv[i]);
	}

	usersList.push_back(newUser);
	return 0;
}

int callbackInt(void* _data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0] != nullptr)
	{
		*static_cast<int*>(_data) = atoi(argv[0]);
		return 0;
	}
	return 1;
}

int callbackFloat(void* _data, int argc, char** argv, char** azColName)
{
	if (argv[0] == nullptr)
		return 0;
	if (argc == 1)
	{
		*static_cast<float*>(_data) = std::stof(argv[0]);
		return 0;
	}
	return 1;
}

int callbackPicture(void* _data, int argc, char** argv, char** azColName)
{
	auto& picturesList = *static_cast<std::list<Picture>*>(_data);
	Picture newPicture(0, "");

	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == ID)
			newPicture.setId(atoi(argv[i]));
		else if (std::string(azColName[i]) == NAME)
			newPicture.setName(argv[i]);
		else if (std::string(azColName[i]) == LOCATION)
			newPicture.setPath(argv[i]);
		else if (std::string(azColName[i]) == CREATION_DATE)
			newPicture.setCreationDate(argv[i]);
	}

	picturesList.push_back(newPicture);
	return 0;
}

int callbackTags(void* _data, int argc, char** argv, char** azColName)
{
	auto& tagsList = *static_cast<std::list<int>*>(_data);

	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == USER_ID)
			tagsList.push_back(atoi(argv[i]));
	}

	return 0;
}
#pragma endregion
/// <summary>
/// Runs query on the database
/// </summary>
/// <param name="query">Query string, send to database</param>
/// <returns>Success status</returns>
bool DatabaseAccess::runQuery(const std::string query)
{
	char* errMessage = nullptr;
	bool SQL_OK = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errMessage) == SQLITE_OK;

	if (errMessage != nullptr)
		std::cerr << "[SQL ERROR] \"" << errMessage << "\"" << std::endl;

	return SQL_OK;
}
/// <summary>
/// runQuery with additional parameters (callback and data)
/// </summary>
/// <param name="query">Query string, send to database</param>
/// <param name="type">Supported types: [0 - Album callback], [1 - User callback], [2 - Int callback], [3 - Float callback], [4 - Picutre callback], [5- Tags callback]</param>
/// <param name="data">Data to callback function</param>
/// <returns>Success status</returns>
bool DatabaseAccess::runCallbackQuery(const std::string query, const int type, void* data)
{
	char* errMessage = nullptr;
	bool SQL_OK = false;

	switch (type) // iterate callback type
	{
		case 0: // Album callback
			data = &this->_albums;
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackAlbum, data, &errMessage) == SQLITE_OK;
			break;
		case 1: // User callback
			data = &this->_users;
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackUser, data, &errMessage) == SQLITE_OK;
			break;
		case 2: // Count callback
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackInt, data, &errMessage) == SQLITE_OK;
			break;
		case 3: // Float count callback
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackFloat, data, &errMessage) == SQLITE_OK;
			break;
		case 4: // Picture callback
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackPicture, data, &errMessage) == SQLITE_OK;
			break;
		case 5: // Tags callback
			SQL_OK = sqlite3_exec(this->_db, query.c_str(), callbackTags, data, &errMessage) == SQLITE_OK;
			break;
	}

	if (errMessage != nullptr)
		std::cerr << "[SQL ERROR] \"" << errMessage << "\"" << std::endl;

	return SQL_OK;
}

bool DatabaseAccess::open()
{
	int fileExist = _access(DB_NAME, 0);
	int res = sqlite3_open(DB_NAME, &this->_db);

	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		return false;
	}
	if (fileExist == -1) // if Database doesn't exist
	{
		// USERS Table
		if (!runQuery("CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL);"))
			return false;

		// ALBUMS Table
		if (!runQuery("CREATE TABLE IF NOT EXISTS ALBUMS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL, CREATION_DATE TEXT NOT NULL, USER_ID INTEGER NOT NULL, FOREIGN KEY (USER_ID) REFERENCES USERS(ID));"))
			return false;

		// PICTURES Table
		if (!runQuery("CREATE TABLE IF NOT EXISTS PICTURES (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL, LOCATION TEXT NOT NULL, CREATION_DATE TEXT NOT NULL, ALBUM_ID INTEGER NOT NULL, FOREIGN KEY(ALBUM_ID) REFERENCES ALBUMS(ID));"))
			return false;

		// TAGS Table
		if (!runQuery("CREATE TABLE IF NOT EXISTS TAGS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, PICTURE_ID INTEGER NOT NULL, USER_ID INTEGER NOT NULL, FOREIGN KEY(PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY(USER_ID) REFERENCES USERS(ID));"))
			return false;
	}
	return true;
}

void DatabaseAccess::close()
{
	sqlite3_close(this->_db);
	this->_db = nullptr;
}

void DatabaseAccess::clear()
{
	this->_albums.clear();
	this->_users.clear();

	this->close(); // close active connection to the DB File, resets _db pointer
	remove(DB_NAME); // remove DB File

	this->open(); // recreates the DB
}

void DatabaseAccess::closeAlbum(Album& pAlbum) {}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	std::string query = "DELETE FROM ALBUMS WHERE NAME = '" + albumName + "' AND USER_ID = '" + std::to_string(userId) + "'; DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'ALBUMS';";
	runQuery(query);
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	std::string query = "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES ((SELECT PICTURES.ID FROM PICTURES INNER JOIN ALBUMS ON PICTURES.ALBUM_ID = ALBUMS.ID WHERE PICTURES.NAME = \"" + pictureName + "\" AND ALBUMS.NAME = \"" + albumName + "\"), " + std::to_string(userId) + ");";
	runQuery(query);
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	std::string query = "DELETE FROM TAGS WHERE PICTURE_ID=(SELECT ID FROM PICTURES WHERE ALBUM_ID=(SELECT ID FROM ALBUMS WHERE NAME= '" + albumName + "') AND NAME = '" + pictureName + "' ) AND USER_ID = '" + std::to_string(userId) + "'; DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'TAGS';";
	runQuery(query);
}

void DatabaseAccess::createUser(User& user)
{
	std::string query = "INSERT INTO USERS (ID, NAME) VALUES ('" + std::to_string(user.getId()) + "', '" + user.getName() + "' );";
	runQuery(query);
}

void DatabaseAccess::deleteUser(const User& user)
{
	// Removes user from users table
	std::string query1 = "DELETE FROM USERS WHERE ID= '" + std::to_string(user.getId()) + "'; DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'USERS';";
	runQuery(query1);

	// Removes tag of the user
	std::string query2 = "DELETE FROM TAGS WHERE USER_ID= '" + std::to_string(user.getId()) + "'; DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'TAGS';";
	runQuery(query2);

	// Removes the pictures of each album which owns the user
	std::string query3 = "DELETE FROM PICTURES WHERE ALBUM_ID = (SELECT ID FROM ALBUMS WHERE USER_ID = '" + std::to_string(user.getId()) + "'); DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'PICTURES';";
	runQuery(query3);

	// Remove the albums of the user
	std::string query4 = "DELETE FROM ALBUMS WHERE USER_ID= '" + std::to_string(user.getId()) + "'; DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'ALBUMS';";
	runQuery(query4);

}

void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	std::string query = "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('" + picture.getName() + "',  '" + picture.getPath() + "', '" + picture.getCreationDate() + "' , (SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' )); ";
	runQuery(query);
}

void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
	std::string query1 = "DELETE FROM PICTURES WHERE NAME = '" + pictureName + "' and ALBUM_ID = (SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "' ); DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'PICTURES';";
	runQuery(query1);

	std::string query2 = "DELETE FROM TAGS WHERE PICTURE_ID=(SELECT ID FROM PICTURES WHERE ALBUM_ID=(SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "') AND NAME='" + pictureName + "'); DELETE FROM SQLITE_SEQUENCE WHERE NAME = 'TAGS';";
	runQuery(query2);
}

const std::list<Album> DatabaseAccess::getAlbums()
{
	this->_albums.clear();

	std::string query = "SELECT USER_ID, NAME, CREATION_DATE FROM ALBUMS;";
	runCallbackQuery(query, 0);
	
	return this->_albums;
}

const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	this->_albums.clear();
	
	std::string query = "SELECT USER_ID, NAME, CREATION_DATE FROM ALBUMS WHERE USER_ID = '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 0);

	return this->_albums;
}

void DatabaseAccess::createAlbum(const Album& album)
{
	std::string query = "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) VALUES ('" + album.getName() + "', '" + album.getCreationDate() + "', '" + std::to_string(album.getOwnerId()) + "');";
	runQuery(query);
}

bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	this->_albums.clear();

	std::string query = "SELECT NAME FROM ALBUMS WHERE USER_ID= '" + std::to_string(userId) + "' AND NAME = '" + albumName + "';";
	runCallbackQuery(query, 0);

	return !this->_albums.empty();
}

Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	this->_albums.clear();
	
	std::string query = "SELECT USER_ID, NAME, CREATION_DATE FROM ALBUMS WHERE NAME = '" + albumName + "';";
	runCallbackQuery(query, 0);
	
	if (this->_albums.size() == 0)
		return Album(0, "");
	else // add pictures to the active album
	{
		int id = 0;
		std::list<int> taggedUsersId;
		std::list<Picture> picturesList;
		picturesList.clear();
		taggedUsersId.clear();

		std::string query3 = "SELECT ID FROM ALBUMS WHERE NAME = '" + albumName + "';";
		runCallbackQuery(query3, 2, &id);

		std::string query4 = "SELECT * FROM PICTURES WHERE ALBUM_ID = " + std::to_string(id) + ";";
		runCallbackQuery(query4, 4, &picturesList);

		for (auto& pic : picturesList) // add pictures
		{
			this->_albums.back().addPicture(Picture(pic.getId(), pic.getName(), pic.getPath(), pic.getCreationDate())); // add picture

			std::string query5 = "SELECT USER_ID FROM TAGS WHERE PICTURE_ID = " + std::to_string(pic.getId()) + ";";
			runCallbackQuery(query5, 5, &taggedUsersId);
			// add tags to picture
			for (auto& userId : taggedUsersId)
			{
				this->_albums.back().tagUserInPicture(userId, pic.getName()); // add tag
			}
		}

		return this->_albums.back();
	}
}

void DatabaseAccess::printAlbums()
{
	this->_albums.clear();

	std::string query = "SELECT USER_ID, NAME, CREATION_DATE FROM ALBUMS;";
	runCallbackQuery(query, 0);

	std::list<Album>::iterator it;

	std::cout << "-- Albums list --" << std::endl;
	for (it = this->_albums.begin(); it != this->_albums.end(); ++it)
	{
		std::cout << "Owner ID: " << it->getOwnerId() << std::endl
			<< "Album name: " << it->getName() << std::endl
			<< "Creation date: " << it->getCreationDate() << std::endl << std::endl;
	}
}

void DatabaseAccess::printUsers()
{
	this->_users.clear();

	std::string query = "SELECT * FROM USERS;";
	runCallbackQuery(query, 1);

	std::list<User>::iterator it;

	std::cout << "-- Users list --" << std::endl;
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		std::cout << "ID: " << it->getId() << std::endl
			<< "User name: " << it->getName() <<  std::endl << std::endl;
	}
}

User DatabaseAccess::getUser(int userId)
{
	this->_users.clear();

	std::string query = "SELECT * FROM USERS WHERE ID = '" + std::to_string(userId) + "';";
	runCallbackQuery(query, 1);

	if (this->_users.size() == 0)
		return User(0, "");
	else
		return this->_users.back();
}

bool DatabaseAccess::doesUserExists(int userId)
{
	this->_users.clear();

	std::string query = "SELECT NAME FROM USERS WHERE ID = '" + std::to_string(userId) + "';";
	runCallbackQuery(query, 1);

	return !this->_users.empty();
}

int DatabaseAccess::countAlbumsOwnedOfUser(const User& user)
{
	this->_albums.clear();

	int count = 0;
	
	std::string query = "SELECT ID FROM ALBUMS WHERE USER_ID = '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 0);

	std::list<Album>::iterator it;
	for (it = this->_albums.begin(); it != this->_albums.end(); ++it)
	{
		count++;
	}

	return count;
}

int DatabaseAccess::countAlbumsTaggedOfUser(const User& user)
{
	int count = 0;

	std::string query = "SELECT COUNT(DISTINCT ALBUMS.ID) FROM ALBUMS INNER JOIN PICTURES ON ALBUMS.ID=PICTURES.ALBUM_ID	INNER JOIN TAGS	ON PICTURES.ID=TAGS.PICTURE_ID WHERE TAGS.USER_ID = '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 2, &count);

	return count;
}

int DatabaseAccess::countTagsOfUser(const User& user)
{
	int count = 0;

	std::string query = "SELECT COUNT(ID) FROM TAGS WHERE USER_ID= '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 2, &count);

	return count;
}

float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
	float avg = 0;

	std::string query = "SELECT 1.0*COUNT(PICTURE_ID)/COUNT(DISTINCT ALBUMS.ID) FROM ALBUMS INNER JOIN PICTURES ON ALBUMS.ID=PICTURES.ALBUM_ID INNER JOIN TAGS ON PICTURES.ID=TAGS.PICTURE_ID WHERE TAGS.USER_ID = '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 3, &avg);

	return avg;
}

User DatabaseAccess::getTopTaggedUser()
{
	this->_users.clear();

	std::string query = "SELECT ID, NAME FROM USERS WHERE ID=(SELECT USER_ID FROM TAGS GROUP BY USER_ID ORDER BY COUNT(ID) DESC LIMIT 1);";
	runCallbackQuery(query.c_str(), 1);
	
	if (this->_users.size() == 0)
		return User(0, "");
	else
		return this->_users.back();
}

Picture DatabaseAccess::getTopTaggedPicture()
{
	std::list<Picture> picturesList;
	picturesList.clear();

	std::string query = "SELECT ID, NAME, LOCATION, CREATION_DATE FROM PICTURES WHERE ID=(SELECT PICTURE_ID FROM TAGS GROUP BY PICTURE_ID ORDER BY COUNT(ID) DESC LIMIT 1);";
	runCallbackQuery(query, 4, &picturesList);

	if (picturesList.size() == 0)
		return Picture(0, "");
	else
		return picturesList.back();
}

std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user)
{
	std::list<Picture> picturesList;
	picturesList.clear();
	
	std::string query = "SELECT PICTURES.ID, NAME, LOCATION, CREATION_DATE FROM PICTURES INNER JOIN TAGS ON TAGS.PICTURE_ID = PICTURES.ID WHERE TAGS.USER_ID = '" + std::to_string(user.getId()) + "';";
	runCallbackQuery(query, 4, &picturesList);

	return picturesList;
}

int DatabaseAccess::lastUserId()
{
	this->_users.clear();

	std::string query = "SELECT ID FROM Users ORDER BY ID DESC LIMIT 1";
	runCallbackQuery(query, 1);

	if (this->_users.size() == 0)
		return 200;
	else
		return this->_users.back().getId();
}

int DatabaseAccess::lastPictureId()
{
	int lastId = 0;

	std::string query = "SELECT ID FROM Pictures ORDER BY ID DESC LIMIT 1";
	runCallbackQuery(query, 2, &lastId);

	if (lastId == 0)
		return 100;
	else
		return lastId + 100;
}
