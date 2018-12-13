#include <utility>


//
// Created by piotr on 03.12.18.
//

#ifndef SK_COMMONS_H
#define SK_COMMONS_H

#include <list>

using namespace std;

class Room;

class User;

struct thread_data;

void startRooms(list<Room *> *rooms);

class User {
public:
    std::__cxx11::string nick;
    std::__cxx11::string password;
    Room *activeRoom;
    pthread_mutex_t* clientMutex;
    int *fd;
    bool logged;

    User(std::__cxx11::string nick, std::__cxx11::string password, bool logged);

    virtual ~User();
};


class Room {
public:
    std::__cxx11::string Name;
    std::__cxx11::string password;
    std::__cxx11::list<User *> *users;

    Room(string Name, string password) : Name(std::move(Name)), password(std::move(password)) {
        users = new list<User *>();
    }

    virtual ~Room();
};


struct thread_data {
    virtual ~thread_data();

    bool *run{};
    pthread_mutex_t **mutex{};
    User *client{};
    list<User *> *users{};
    list<Room *> *rooms{};
    int * fd;
};


#endif //SK_COMMONS_H
