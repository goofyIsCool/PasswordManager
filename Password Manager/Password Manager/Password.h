#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::endl;

class Password {

	string name;
	string password;
	string category;
	string URL;
	string login;

	bool optional;

public:


	Password() = default;

	Password(std::vector<string> data);

	Password(string name, string password, string category);

	Password(string name, string password, string category, string URL, string login);

	~Password();

	string RandomPassword(int numberOfCharacters, bool UpperLowercase, bool specialChars);

	string toString();

	string getName() const;

	void setName(string name);

	string getPassword() const;

	void setPassword(string password);

	string getCategory() const;

	void setCategory(string category);

	string getURL() const;

	void setURL(string URL);

	string getLogin() const;

	void setLogin(string Login);

};

std::ostream& operator<<(std::ostream& strm, const Password& p);