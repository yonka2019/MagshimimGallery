#pragma once
#include <string>
#include <iostream>

class User
{
public:
	
	User(int id, const std::string& name);

	int getId() const;
	void setId(int id);

	const std::string& getName() const;
	void setName(const std::string& name);

	bool operator==(const User& other) const;
	bool operator==(int id) const;
	bool operator<(const User& other) const;
	friend std::ostream& operator<<(std::ostream& str, const User& other);

private:
	int m_id;
	std::string m_name;
};