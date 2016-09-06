#pragma once
#include <string>

enum UserFlags {
	NONE = 0x0,
	BASICS = 0x1,
	LOCATION = 0x2
};

inline UserFlags operator|(UserFlags a, UserFlags b) {
	return UserFlags(int(a) | int(b));
}

class userid_t {
	int value;
public:
	explicit userid_t(int v) : value(v) { }

	operator int() {
		return value;
	}
};

class User {
public:
	userid_t userid;
	User(userid_t userid) : userid(userid) {
	}
};

class UserBasics {
public:
	static const UserFlags flag = UserFlags::BASICS;
	std::string name;
	int age;
	bool loaded;
	UserBasics() : name("Name"), age(18), loaded(false) {}
	static void load(UserBasics& userBasics) {}
};

class UserLocation {
public:
	static const UserFlags flag = UserFlags::LOCATION;
	std::string city;
	bool loaded;
	UserLocation() : city("City"), loaded(false) {}
	static void load(UserLocation& userLocation) {}
};