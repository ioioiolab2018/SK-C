//
// Created by piotr on 03.12.18.
//

#include <list>
#include <iostream>
#include "Commons.h"

using namespace std;


User::~User(){
    delete(fd);
}


Room::~Room() {
    delete (users);
}

thread_data::~thread_data() {
    delete (run);
}

void startRooms(list<Room *> *rooms) {
    rooms->push_back(new Room("FirstRoom", ""));
    rooms->push_back(new Room("LastRoom", ""));
}

User::User(std::__cxx11::string nick, std::__cxx11::string password, bool logged) : nick(std::move(nick)),
                                                                                    password(std::move(
                                                                                            password)),
                                                                                    activeRoom(nullptr),
                                                                                    logged(logged),
                                                                                    fd(new int(0)) {}
