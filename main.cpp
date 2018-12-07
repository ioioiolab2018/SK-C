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
    list<User> users;
    list<Room> rooms;
    startUsers(&users);
    startRooms(&rooms);
    cout << "USERS #############" << endl;
    for (auto &user : users) {
        cout << user.nick << endl;
    }
    cout << "ROOMS #############" << endl;
    for (auto &room : rooms) {
        cout << room.Name << endl;
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
            struct thread_data data;
            data.users = &users;
            data.rooms = &rooms;
            data.client = nullptr;
            data.fd = connFd;
            pthread_create(&threadA[noThread], nullptr, clientConectionHandler, (void *) &data);

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

    auto *th_data = (struct thread_data *) data;

    pthread_detach(pthread_self());
    cout << "New client, thread No: " << pthread_self() << endl;

    bool run = true;
    ssize_t r = 0;
    char buf[BUFF_SIZE];
    string message;
    string lastBuffer;
    bool next= true;

    while (run) {
        bzero(buf, BUFF_SIZE + 1);
        if(next) {
            r = read(th_data->fd, buf, BUFF_SIZE);
        }
        if (r > 0 || !next) {
            message = buf;
            message = lastBuffer.append(message);
            ssize_t start=message.find(START);
            if(start == string::npos){
                next= true;
                lastBuffer=" ";
                continue;
            }
            message.erase(0, static_cast<unsigned long>(start));

            ssize_t end =message.find(END);
            if(end  == string::npos){
                lastBuffer = message;
                continue;
            }
            else{
                next=false;
                lastBuffer=message.substr(static_cast<unsigned long>(end)+1, message.length());
                message.erase(static_cast<unsigned long>(end), message.length()-1);
                message.erase(0, 1);
            }
        } else {
            continue;
        }
        trim_inplace(message);
;
        vector<std::string> words;
        cout << message <<" RESZTA: "<< lastBuffer<< endl;
        continue;
        split1(buf, words);
        if (!words.empty()) {
            if (words[0] == "login") {
                cout << "logowanie #############" << endl;
                for (auto &user : *(th_data->users)) {
                    if (user.nick == words[1]) {
                        if (user.password == words[2]) {
                            user.logged = true;
                            user.fd = th_data->fd;
                            th_data->client = &user;
                            cout << "zalogowano user 1" << endl;
                            char *od = const_cast<char *>("Zalogowano");
                            write(user.fd, od, 9);
                        } else {
                            cout << user.nick << " zle hasło" << endl;
                        }
                    }
                }
            }
        }
        if (!words.empty()) {
            if (words[0] == "enter") {
                cout << "wchodzenie #############" << endl;
                for (auto &room : *(th_data->rooms)) {
                    if (room.Name == words[1]) {
                        th_data->client->activeRoom = &room;
                        (room.users)->push_back(*(th_data->client));
                    }
                }
            }
        }

        if (!words.empty()) {
            if (words[0] == "message") {
                for (const auto &user: *(th_data->client->activeRoom->users)) {
                    if (th_data->client->nick != user.nick) {
                        char *od = const_cast<char *>(words[1].c_str());
                        write(user.fd, od, sizeof(od));
                    }
                }
            }
        }

        string tester(buf);
        if (tester == "exit")
            break;
    }
    cout << "\nClosing thread and conn" << endl;
    close(th_data->fd);

}

