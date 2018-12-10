//
// Created by piotr on 03.12.18.
//

#include <list>
#include <iostream>
#include "Commons.h"

using namespace std;


void startRooms(list<Room *> *rooms) {
    rooms->push_back(new Room("FirstRoom", ""));
    rooms->push_back(new Room("LastRoom", ""));
}

User::User(std::__cxx11::string nick, std::__cxx11::string password, bool logged) : nick(std::move(nick)),
                                                                                    password(std::move(
                                                                                            password)),
                                                                                    activeRoom(NULL),
                                                                                    logged(logged),
                                                                                    fd(-1) {}
