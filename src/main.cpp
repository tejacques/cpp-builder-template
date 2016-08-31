// TemplateMagic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "builder.h"

void takesBuilt(const BuiltType<UserBasics>& built)
{
    userid_t id = built.userid;
    std::cout << "5: userid: " << id << ", name: " << built.name << ", age: " << built.age << "\n";
}

void testfn()
{
    Builder<UserBasics, UserLocation> builder;
    auto built = builder.load(userid_t(314));
    built.name = "Built";
    built.age = 120;
    std::cout << "1: userid: " << built.userid << ", name: " << built.name << ", age: " << built.age << "\n";
    BuiltType<UserBasics> basics = built;
    std::cout << "1: userid: " << basics.userid << ", name: " << basics.name << ", age: " << basics.age << "\n";
    takesBuilt(built);
    loadAllTypes(built);

	const int j = 5;
	auto built2 = builder.load(userid_t(314));;
	built2.age = 1234;

    // Missing UserLocation
    // built = basics;
}

int main() {
	auto basicsBuilder = Builder<>()
		.load<UserBasics>();

	auto builder = Bldr()
		.load<UserBasics>()
		.load<UserLocation>()
        .load<UserBasics>();

	Bldr
		::with<UserLocation>
		::with<UserBasics>
		::with<UserLocation>
		::without<UserLocation>
		a;


	using UserLocBuilder2 = Bldr::with<UserLocation>;
	userid_t userid = userid_t(1234);
	auto usr = a.load(userid);
	auto basicsAndLocation = builder.load(userid);
	basicsAndLocation.age = 10;
	basicsAndLocation.city = "Location Name";
	auto builtUser = builder.load(userid);
	BuiltType<UserBasics> justBasics = basicsAndLocation;

    std::cout << "3: userid: " << builtUser.userid << ", name: " << builtUser.name << ", age: " << builtUser.age << ", city: " << builtUser.city << "\n";

	BuiltType<UserBasics, UserLocation>::withoutT<UserLocation> foo(userid_t(0));
	//auto basics2 = basicsAndLocation.without<UserLocation>();
	//basics2.city = 0xd00f;


    std::vector<userid_t> userids = {
        userid_t(1),
        userid_t(2),
        userid_t(3),
    };
    for (const auto& uid : userids) {
        auto built = builder.load(uid);
        std::cout << "4: userid: " << built.userid << ", name: " << built.name << ", age: " << built.age << ", city: " << built.city << "\n";
	}

	testfn();

	return 0;
}