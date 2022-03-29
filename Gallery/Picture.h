#pragma once
#include "User.h"
#include <set>
#include <string>
#include <memory>
#include <iomanip>

class Picture
{
public:
	Picture(int id, const std::string& name);
	Picture(int id, const std::string& name, const std::string& pathOnDisk, const std::string& creationDate);

	int getId() const;
	void setId(int id);

	const std::string& getName() const;
	void setName(const std::string& name);

	const std::string& getPath() const;
	void setPath(const std::string& location);

	const std::string& getCreationDate() const;
	void setCreationDate(const std::string& creationTime);
	void setCreationDateNow();

	bool isUserTagged(const User& user) const;
	bool isUserTagged(int userId) const;
	void tagUser(const User& user);
	void tagUser(int userId);
	void untagUser(const User& user);
	void untagUser(int userId);
	int getTagsCount() const;

	const std::set<int>& getUserTags() const;

	bool operator==(const Picture& other) const;
	friend std::ostream& operator<<(std::ostream& strout, const Picture& object);

private:
	int m_pictureId;
	std::string m_name;
	std::string m_pathOnDisk;
	std::string m_creationDate;
	std::set<int> m_usersTags;
};
