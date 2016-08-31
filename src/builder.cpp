#include "stdafx.h"
#include "builder.h"

// == Compile Time Tests ==
static_assert(ContainsType<UserBasics, TypeList<UserBasics>>::value,
	"TypeList<UserBasics> must contain UserBasics");
static_assert(ContainsAllTypes<TypeList<UserBasics>, TypeList<UserBasics>>::value,
	"TypeList<UserBasics> must contain all of TypeList<UserBasics>");
static_assert(ContainsAllTypes<TypeList<UserBasics>, TypeList<UserBasics, UserLocation>>::value,
	"TypeList<UserBasics, UserLocation> must contain all of TypeList<UserBasics>");
static_assert(ContainsAllTypes<TypeList<UserBasics, UserLocation>, TypeList<UserBasics, UserLocation>>::value,
	"TypeList<UserBasics, UserLocation> must contain all of TypeList<UserBasics, UserLocation>");

static_assert(!ContainsAllTypes<TypeList<UserBasics, UserLocation>, TypeList<UserBasics>>::value,
	"TypeList<UserBasics> must not contain all of TypeList<UserBasics, UserLocation>");
static_assert(!ContainsAllTypes<TypeList<UserLocation>, TypeList<UserBasics>>::value,
	"TypeList<UserBasics> must not contain all of TypeList<UserLocation>");