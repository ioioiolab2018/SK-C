//
// Created by piotr on 03.12.18.
//

#include <list>
#include <iostream>
#include "Commons.h"

using namespace std;


void startUsers(list<User> *users) {
    User user1 = User("piotr", "admin", false);
    users->push_back(user1);
    User user2 = User("pawel", "admin", false);
    users->push_back(user2);
    User user3 = User("monika", "admin", false);
    users->push_back(user3);

}

void startRooms(list<Room> *rooms) {
    Room room1 = Room("pok1", "admin");
    rooms->push_back(room1);
    Room room2 = Room("pok2", "admin");
    rooms->push_back(room2);
}