#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by piotr on 03.12.18.
//

#ifndef SK_COMMONS_H
#define SK_COMMONS_H

#include <list>

#define START string("$")
#define END "#"
#define LOGIN "login"
#define MESSAGE "message"
#define ENTER "enter"
#define CREATE "create"
#define ROOM_LIST "rooms"
#define USER_LIST "users"
#define  EXIT "leave"
#define OK "ok"
#define NOT_OK "notOk"
#define  LOGOUT "logout"
#define BUFF_SIZE 100
#define DELIMITER ":"

using namespace std;

class Room;

class User;

class Message;

struct thread_data;


void startRooms(list<Room *> *rooms);

class User {
public:
    std::__cxx11::string nick;
    std::__cxx11::string password;
    Room *activeRoom;
    int fd;
    bool logged;

    User(std::__cxx11::string nick, std::__cxx11::string password, bool logged) : nick(std::move(nick)),
                                                                                  password(std::move(
                                                                                          password)),
                                                                                  activeRoom(NULL),
                                                                                  logged(logged),
                                                                                  fd(-1) {}
};


class Room {
public:
    std::__cxx11::string Name;
    std::__cxx11::string password;
    std::__cxx11::list<User *> *users;

    Room(string Name, string password) : Name(std::move(Name)), password(std::move(password)) {
        users = new list<User *>();
    }
};

struct thread_data {
    User *client;
    list<User *> *users;
    list<Room *> *rooms;
    int fd;
};


#endif //SK_COMMONS_H
