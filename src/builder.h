#pragma once
#include "stdafx.h"
#include "user.h"
#include "type_utils.h"
#include <typeinfo>
#include <iostream>


template<class ...Types>
class Builder;

template<class ...Types>
class BuiltType;

template<class ...Types>
struct BuilderType
{
};

template<class ...Types>
struct BuilderType<TypeList<Types...> >
{
    typedef Builder<Types...> Builder;
    typedef BuiltType<Types...> Built;
};

typedef Builder<> Bldr;

template<class Built1, class Built2, class ...Types>
struct Copier
{
    static void copy(Built1& built, const Built2& copy);
};

template<class Built1, class Built2, class T, class ...Types>
struct Copier<Built1, Built2, T, Types...>
{
    static void copy(Built1& built, const Built2& copy)
    {
        (static_cast<T *>(&built))->operator=(copy);
        std::cout << "Typename T: " << typeid(T).name() << "\n";
        Copier<Built1, Built2, Types...>::copy(built, copy);
    }
};

template<class Built1, class Built2>
struct Copier<Built1, Built2>
{
    static void copy(Built1& built, const Built2& copy)
    {
        // empty
    }
};

template<class ...Types>
class BuiltType : public User, public Types ...
{
public:
    template<typename T>
    using withoutT = typename BuilderType<typename RemoveType<T, Types...>::type>::Built;

    BuiltType(userid_t userid) : User(userid) {};

    template<class ...TArgs>
    BuiltType(const BuiltType<TArgs...>& other) : User(other.userid)
    {
        static_assert(
            ContainsAllTypes<TypeList<Types...>, TypeList<TArgs...> >::value,
            "Attempting to convert to incompatible types"
        );
        Copier<BuiltType, BuiltType<TArgs...>, User, Types...>::copy(*this, other);
    }
};

template<class ...Types>
class FlagLoader
{
public:
    static UserFlags getFlags();
};

template<class T, class ...Types>
class FlagLoader<T, Types...>
{
public:
    static UserFlags getFlags()
    {
        return T::flag | FlagLoader<Types...>::getFlags();
    };
};

template<class T>
class FlagLoader<T>
{
public:
    static UserFlags getFlags()
    {
        return T::flag;
    };
};

template<class Built, class ...Types>
struct Loader
{
    static void loadTypes(Built& built);
};

template<class Built, class T, class ...Types>
struct Loader<Built, T, Types...>
{
    static void loadTypes(Built& built)
    {
        UserFlags flags = FlagLoader<T, Types...>::getFlags();
        T *t = static_cast<T *>(&built);
        if (!t->loaded) {
            //printf("Loading flag: %d status: %d\n", T::flag, static_cast<T>(built).loaded);
            T::load(built);
            t->loaded = true;
            //printf("Loaded: %d\n", static_cast<T>(built).loaded);
        }

        Loader<Built, Types...>::loadTypes(built);
    }
};

template<class Built>
struct Loader<Built>
{
    static void loadTypes(Built& built) {}
};

template<class ...Types>
void loadAllTypes(BuiltType<Types...>& built)
{
    Loader<BuiltType<Types...>, Types...>::loadTypes(built);
}

template<class ...Types>
class Builder
{
public:
    template<class T>
    using with = typename BuilderType<typename AddType<T, TypeList<Types...> >::type>::Builder;

    template<class T>
    using without = typename BuilderType<typename RemoveType<T, Types...>::type>::Builder;

    template<class U>
    with<U> load()
    {
        with<U> result;
        return result;
    }

    BuiltType<Types...> load(userid_t userid)
    {
        BuiltType<Types...> result(userid);
        loadAllTypes(result);
        return result;
    }

    std::vector<BuiltType<Types...> > load(std::vector<userid_t>& userids)
    {
        std::vector<BuiltType<Types...> > results;
        results.reserve(userids.size());
        for (const auto& userid : userids) {
            results.push_back(this->load(userid));
        }
        return results;
    }
};