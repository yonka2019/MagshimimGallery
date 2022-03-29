#pragma once
#include <sstream>
#include "MyException.h"

class ItemNotFoundException : public MyException {
public:
	ItemNotFoundException(const std::string& item, int user_id) : MyException(item), 
																  m_id(user_id), m_name("") {}
	ItemNotFoundException(const std::string& item, const std::string& name) : MyException(item),
																			  m_id(-1),m_name(name){}
	virtual ~ItemNotFoundException() noexcept {}
	virtual const char* what() const noexcept {
		std::stringstream s;
		if(m_id > -1)
			s << m_message << " with id " << m_id << " does not exist";
		else
			s << m_message << " with name " << m_name << " does not exist";

		return s.str().c_str();
	}
protected:
	int m_id;
	std::string m_name;
};

