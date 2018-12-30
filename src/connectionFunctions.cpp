//


#include <iostream>
#include <unistd.h>
#include "connectionFunctions.h"
#include "serverStatements.h"
#include "messageTools.h"

//
// Created by piotr on 11.12.18.

// wyslanie listy pokoi do wybranego uzytkownika
void roomListFunction(thread_data *th_data, int *fd);

void createRoomFunction(thread_data *th_data, string &message) {
    string reply;
    pthread_mutex_t *roomsMutex = th_data->mutex[1];
    bool ok = true;
    string name = get_first_word(message, DELIMITER);
    if (name.empty()) {
        ok = false;
    }

    if (ok) {
        pthread_mutex_lock(roomsMutex);
        for (auto &room : *(th_data->rooms)) {
            if (room->Name == name) {
                ok = false;
                break;
            }
        }
    }

    if (ok) {
        th_data->rooms->push_back(new Room(name, " "));
        reply = START + CREATE + DELIMITER + OK + END;
        sendToClient(th_data->fd, th_data->client, th_data, reply);
        for (auto &user : *(th_data->users)) {
            roomListFunction(th_data, user->fd);
        }
    } else {
        reply = START + CREATE + DELIMITER + NOT_OK + END;
        sendToClient(th_data->fd,  th_data->client, th_data, reply);
    }
    pthread_mutex_unlock(roomsMutex);
}

// wyslanie listy pokoi do uzytkownika pobranego z th_data
void roomListFunction(thread_data *th_data) {
    string reply;
    reply = START + ROOM_LIST + DELIMITER;

    for (auto &room : *(th_data->rooms)) {
        reply += room->Name + ";" + to_string(room->users->size()) + " ";
    }

    reply += END;
    sendToClient(th_data->fd,  th_data->client,th_data, reply);
}

void roomListFunction(thread_data *th_data, int *fd) {
    string reply;
    reply = START + ROOM_LIST + DELIMITER;

    for (auto &room : *(th_data->rooms)) {
        reply += room->Name + ";" + to_string(room->users->size()) + " ";
    }
    reply += END;
    sendToClient(fd, th_data->client, th_data, reply);
}


// wysylanie listy uzytkownikow do domyslnego uzytkownika
void userListFunction(thread_data *th_data, string &message) {
    string reply;
    string name = get_first_word(message, DELIMITER);
    reply = START + USER_LIST + DELIMITER;

    for (auto &room : *(th_data->rooms)) {
        if (room->Name == name) {
            for (auto &user : *(room->users)) {
                reply += user->nick + " ";
            }
        }
    }
    reply += END;
    sendToClient(th_data->fd,  th_data->client, th_data, reply);

}


// wysyłanie listy uzytkownikow do dowolnego uzytkownika
void userListFunction(thread_data *th_data, int *fd, string &message) {
    string reply;
    string name = get_first_word(message, DELIMITER);
    reply = START + USER_LIST + DELIMITER;

    for (auto &room : *(th_data->rooms)) {
        if (room->Name == name) {
            for (auto &user : *(room->users)) {
                reply += user->nick + " ";
            }
        }
    }
    reply += END;
    sendToClient(fd, th_data->client, th_data, reply);
}

void messageFunction(thread_data *th_data,
                     string &message) {//przesylanie wiadomosci do wszystkich uzykownikow ktorzy sa w tym samym pokoju
    string reply;
    if (th_data->client->activeRoom != nullptr) {
        message = get_first_word(message, DELIMITER);
        reply = START + MESSAGE + DELIMITER + message + END;

        for (auto &user: *(th_data->client->activeRoom->users)) {
            if (user->logged && user->nick != th_data->client->nick) {
                cout << reply << endl;
                sendToClient(user->fd, user, th_data, reply);
            }
        }
    }
}

void enterFunction(thread_data *th_data, string &message) {
    pthread_mutex_t *usersMutex = th_data->mutex[0];
    pthread_mutex_t *roomsMutex = th_data->mutex[1];
    bool ok = false;
    string reply;
    string name = get_first_word(message, DELIMITER);


// dodanie uzytkownika do pokoju i dodanie pokoju jako aktywnego do uzytkownika
    pthread_mutex_lock(roomsMutex);
    for (auto &room : *(th_data->rooms)) {
        if (room->Name == name) {
            bool okName = true;
            for (auto &user : *(room->users)) {
                if (user->nick == th_data->client->nick) {
                    okName = false;
                    break;
                }
            }
            if (okName) {
                room->users->push_back(th_data->client);
            }
            pthread_mutex_lock(th_data->client->clientMutex);
            th_data->client->activeRoom = room;
            pthread_mutex_unlock(th_data->client->clientMutex);

            reply = START + ENTER + DELIMITER + OK + END;
            sendToClient(th_data->fd, th_data->client,  th_data, reply);
            for (auto &user : *(room->users)) {
                userListFunction(th_data, user->fd, name);
            }
            for (auto &user : *(th_data->users)) {
                roomListFunction(th_data, user->fd);
            }

            ok = true;
            break;
        }
    }
    pthread_mutex_unlock(roomsMutex);


// blad, jesli nie ma pokoju o podanej nazwie
    if (!ok) {
        reply = START + ENTER + DELIMITER + NOT_OK + END;
        sendToClient(th_data->fd, th_data->client, th_data, reply);
    }
}

void logoutFunction(thread_data *th_data, bool &connected) {
    string reply;
    pthread_mutex_t *usersMutex = th_data->mutex[0];
    // usuniecie uzytkownika z pokoju w ktorym sie znajdowal

    exitFunction(th_data); // wyjscie z pokoju

    pthread_mutex_lock(usersMutex);
    // usuniecie uzytkownika z listy users
    th_data->users->remove_if([th_data](User *u) {
        if (u->nick == th_data->client->nick) {
            delete (u);
            return true;
        } else
            return false;
    });
    pthread_mutex_unlock(usersMutex);

    //odeslanie potwierdzenia
    reply = START + LOGOUT + DELIMITER + OK + END;
    sendToClient(th_data->fd, th_data->client,  th_data, reply);

    connected = false;
}

void loginFunction(thread_data *th_data, string &message) {
    string reply;
    string nick = get_first_word(message, DELIMITER);
    pthread_mutex_t *usersMutex = th_data->mutex[0];
    bool ok = true;

    pthread_mutex_lock(usersMutex);
    //sprawdzenie czy istnieje juz uzytkownik z takim nickiem
    for (auto &user : *(th_data->users)) {
        if (user->nick == nick) {
            cout << "JEST JUZ TAKI UZYTKOWNIK" << endl;
            reply = START + LOGIN + DELIMITER + NOT_OK + END;
            ok = false;
            continue;
        }
    }

    // dodanie nowego uzytkownika do listy users
    if (ok) {
        User *newUser = new User(nick, " ", true);
        newUser->fd = new int(*th_data->fd);
        th_data->client = newUser;
        newUser->clientMutex = new pthread_mutex_t;
        pthread_mutex_init(newUser->clientMutex, NULL);

        th_data->users->push_back(newUser);

        pthread_mutex_unlock(usersMutex);

        reply = START + LOGIN + DELIMITER + OK + END;
        cout << "Logged: " << nick << endl;
    }else{
        pthread_mutex_unlock(usersMutex);
    }

    sendToClient(th_data->fd, th_data->client, th_data, reply);
    roomListFunction(th_data); // wysłanie listy pokojow

}

// usuniecie uzytkownika z pokoju w ktorym sie znajdowal
void exitFunction(thread_data *th_data) {
    string reply;
    pthread_mutex_t *roomsMutex = th_data->mutex[1];

    pthread_mutex_lock(roomsMutex);
    if (th_data->client->activeRoom != nullptr) {
        th_data->client->activeRoom->users->remove_if(
                [th_data](User *u) { return u->nick == th_data->client->nick; });

        for (auto &user : *(th_data->client->activeRoom->users)) {
            userListFunction(th_data, user->fd, th_data->client->activeRoom->Name);
        }


        //usuniecie pustyc pokojow
        if (th_data->client->activeRoom->users->empty()) {
            th_data->rooms->remove_if(
                    [th_data](Room *r) {
                        if (r->Name == th_data->client->activeRoom->Name) {
                            delete (r);
                            return true;
                        } else return false;
                    });
        }

        for (auto &user : *(th_data->users)) {
            roomListFunction(th_data, user->fd);
        }

        th_data->client->activeRoom = nullptr;
    }
    pthread_mutex_unlock(roomsMutex);

    reply = START + EXIT + DELIMITER + OK + END;
    sendToClient(th_data->fd, th_data->client, th_data, reply);


}

void sendToClient(int *fd,User * user,  thread_data *th_data, string &reply) {
    if(*fd==-1){
        return;
    }
    bool lock = false;

    if(user != nullptr){
        lock = true;
    }
    pthread_mutex_t *clientMutex;


    if (lock) clientMutex = user->clientMutex;
    cout << reply << endl;
    if (lock) pthread_mutex_lock(clientMutex);

    bool writing = true;
    int error = 0;
    while (writing && !reply.empty()) {
        ssize_t n = write(*fd, reply.c_str(), reply.size());
        if (n < 0 ) {
            error++;
            if (error > 5) {
                cerr << "Bład połącznia z klientem!" << endl;
                if (lock) pthread_mutex_unlock(clientMutex);
                if (*th_data->fd == *fd) {
                    *th_data->run = false;
                } else {
                    *fd = -1;
                }
                return;
            }
        } else if (n < reply.size()) {
            reply.erase(0, static_cast<unsigned long>(n));
        } else {
            writing = false;
        }
    }
   if(lock) pthread_mutex_unlock(clientMutex);

}