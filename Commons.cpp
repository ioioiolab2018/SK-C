//
// Created by piotr on 03.12.18.
//

#include <list>
#include <iostream>
#include "Commons.h"

using namespace std;


void startUsers(list<User> *users) {
}

void startRooms(list<Room *> *rooms) {
    rooms->push_back(new Room("pok1", "admin"));
    rooms->push_back(new Room("pok2", "admin"));
}