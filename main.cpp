#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <list>
#include "Commons.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "stringCommons.h"

using namespace std;


void *clientConectionHandler(void *);


int main(int argc, char *argv[]) {
    int pId, portNo, listenFd;

    socklen_t len; //store size of the address
    bool loop = false;
    bool run = true;

    //zainicjowana lista użytkownikow
    auto *users = new list<User *>;
    auto *rooms = new list<Room *>;
    startRooms(rooms);
    cout << "USERS #############" << endl;
    for (auto &user : *users) {
        cout << user->nick << endl;
    }
    cout << "ROOMS #############" << endl;
    for (auto room : *rooms) {
        cout << room->Name << endl;
    }

    struct sockaddr_in svrAdd;
    struct sockaddr_in clntAdd;


    pthread_t threadA[10];

    if (argc < 2) {
        cerr << "Syntax : ./server <port>" << endl;
        return 0;
    }

    portNo = atoi(argv[1]);

    if ((portNo > 65535) || (portNo < 2000)) {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);;

    if (listenFd < 0) {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    bzero((char *) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(static_cast<uint16_t>(portNo));

    //bind socket
    if (bind(listenFd, (struct sockaddr *) &svrAdd, sizeof(svrAdd)) < 0) {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(listenFd, 5);

    len = sizeof(clntAdd);

    //liczba klientow/wątków
    int noThread = 0;

    while (run) {
        if (noThread < 10) {
            cout << "Listening" << endl;
            //this is where client connects. svr will hang in this mode until client conn
            int connFd = accept(listenFd, (struct sockaddr *) &clntAdd, &len);

            if (connFd < 0) {
                cerr << "Cannot accept connection" << endl;
                return 0;
            } else {
                cout << "Connection successful" << endl;
            }
            struct thread_data *data = new struct thread_data;
            data->users = users;
            data->rooms = rooms;
            data->client = nullptr;
            data->fd = connFd;
            pthread_create(&threadA[noThread], nullptr, clientConectionHandler, (void *) data);

            noThread++;
        }
    }
}

template<class Container>
void split1(const std::string &str, Container &cont) {
    istringstream iss(str);
    copy(std::istream_iterator<std::string>(iss),
         istream_iterator<std::string>(),
         back_inserter(cont));
}


void *clientConectionHandler(void *data) {
    bool connected = true;
    auto *th_data = (struct thread_data *) data;

    pthread_detach(pthread_self());
    cout << "New client, thread No: " << pthread_self() << endl;

    bool run = true;
    ssize_t r = 0;
    char buf[BUFF_SIZE];
    string message;
    string lastBuffer;
    bool next = true;

    while (run) {
        bzero(buf, BUFF_SIZE + 1);
        if (next) {
            r = read(th_data->fd, buf, BUFF_SIZE);
        }
        if (r > 0 || !next) {
            message = buf;
            message = lastBuffer.append(message);
            ssize_t start = message.find(START);
            if (start == string::npos) {
                next = true;
                lastBuffer = " ";
                continue;
            }
            message.erase(0, static_cast<unsigned long>(start));

            ssize_t end = message.find(END);
            if (end == string::npos) {
                lastBuffer = message;
                continue;
            } else {
                next = false;
                lastBuffer = message.substr(static_cast<unsigned long>(end) + 1, message.length());
                message.erase(static_cast<unsigned long>(end), message.length() - 1);
                message.erase(0, 1);
            }
        } else {
            continue;
        }
        trim_inplace(message);
        vector<std::string> words;
        //TEST
        //cout << message <<" RESZTA: "<< lastBuffer<< endl;
        //cout << "pierwsze slowo: " << get_first_word(message, DELIMITER) << endl;
        //cout << message << endl;
        cout << "wiadomosc: " << message << endl;
        string command = get_first_word(message, DELIMITER);
        bool send = false;
        string reply;

        if (command == LOGIN) {
            string nick = get_first_word(message, DELIMITER);
            for (auto &user : *(th_data->users)) {
                if (user->nick == nick) {
                    cout << "JEST JUZ TAKI UZYTKOWNIK" << endl;
                    reply = START + LOGIN + DELIMITER + NOT_OK + END;
                    send = true;
                    continue;
                }

            }
            User *newUser = new User(nick, " ", true);
            newUser->fd = th_data->fd;
            th_data->client = newUser;
            th_data->users->push_back(newUser);
            reply = START + LOGIN + DELIMITER + OK + END;
            send = true;
            cout << "Logged" << nick << endl;

        } else if (command == LOGOUT) {
            th_data->users->remove_if([th_data](User *u) { return u->nick == th_data->client->nick; });
            if (th_data->client->activeRoom != nullptr) {
                th_data->client->activeRoom->users->remove_if(
                        [th_data](User *u) { return u->nick == th_data->client->nick; });
            }
            reply = START + LOGOUT + DELIMITER + OK + END;
            send = true;
            connected = false;

        } else if (command == ENTER) {
            bool ok = false;
            string name = get_first_word(message, DELIMITER);
            for (auto &room : *(th_data->rooms)) {
                if (room->Name == name) {
                    room->users->push_back(th_data->client);
                    th_data->client->activeRoom = room;
                    reply = START + ENTER + DELIMITER + OK + END;
                    send = true;
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                reply = START + ENTER + DELIMITER + NOT_OK + END;
                send = true;
            }

        } else if (command == MESSAGE) {
            if (th_data->client->activeRoom != nullptr) {
                message = get_first_word(message, DELIMITER);
                reply = START + MESSAGE + DELIMITER + message + END;
                for (auto &user: *(th_data->client->activeRoom->users)) {
                    if (user->logged && user->nick != th_data->client->nick) {
                        cout << reply << endl;
                        ssize_t n = write(user->fd, reply.c_str(), reply.size());
                    }
                }
            }

        } else if (command == CREATE) {
            bool ok = false;
            string name = get_first_word(message, DELIMITER);
            for (auto &room : *(th_data->rooms)) {
                if (room->Name == name) {
                    ok = true;
                    break;
                }
            }
            if (ok) {
                th_data->rooms->push_back(new Room(name, " "));
                reply = START + CREATE + DELIMITER + NOT_OK + END;
                send = true;
            } else {
                reply = START + CREATE + DELIMITER + OK + END;
                send = true;
            }

        } else if (command == ROOM_LIST) {
            send = true;
            reply = START + ROOM_LIST + DELIMITER;
            for (auto &room : *(th_data->rooms)) {
                reply += room->Name + ";" + to_string(room->users->size()) + " ";
            }
            reply = reply.substr(0, reply.size() - 1);
            reply += END;

        } else if (command == EXIT) {
            if (th_data->client->activeRoom != nullptr) {
                th_data->client->activeRoom->users->remove_if(
                        [th_data](User *u) { return u->nick == th_data->client->nick; });
                th_data->client->activeRoom = nullptr;
            }
            reply = START + EXIT + DELIMITER + OK + END;
            send = true;
        } else if (command == USER_LIST) {
            send = true;
            reply = START + USER_LIST + DELIMITER;
            for (auto &user : *(th_data->users)) {
                reply += user->nick + " ";
            }
            reply = reply.substr(0, reply.size() - 1);
            reply += END;
        }

        if (send) { ;
            cout << reply << endl;
            ssize_t n = write(th_data->fd, reply.c_str(), reply.size());
            send = false;
        }

    }
    cout << "\nClosing thread and conn" << endl;
    close(th_data->fd);

}

