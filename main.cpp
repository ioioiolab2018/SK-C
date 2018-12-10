#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <list>

#include "Commons.h"
#include "connectionHandler.h"
#include "serverStatements.h"

using namespace std;

void *serverManagementFunction(void *run);


int main() {
    int port, serverFd;
    pthread_mutex_t usersMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t roomsMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t connectionMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t *mutex[3] = {&usersMutex, &roomsMutex, &connectionMutex};

    socklen_t len; //store size of the address
    bool run = true;


    auto *users = new list<User *>;
    auto *rooms = new list<Room *>;
    startRooms(rooms);


    struct sockaddr_in svrAdd{};
    struct sockaddr_in clntAdd{};


    list<pthread_t *> threads;

    port = PORT;

    //create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);;

    if (serverFd < 0) {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    bzero((char *) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(static_cast<uint16_t>(port));

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(serverFd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    //bind socket
    if (bind(serverFd, (struct sockaddr *) &svrAdd, sizeof(svrAdd)) < 0) {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(serverFd, 20);

    len = sizeof(clntAdd);

    //liczba klientow/wątków
    pthread_t serverThread;
    pthread_t serverThread2;
    pthread_create(&serverThread, nullptr, serverManagementFunction, (void *) &run);

    list<bool *> clientThreadIsRun;
    cout << "Listening" << endl;
    while (run) {
        //this is where client connects. svr will hang in this mode until client conn
        int connFd = accept(serverFd, (struct sockaddr *) &clntAdd, &len);

        if (connFd < 0) {
            continue;
        } else {
            cout << "Connection successful" << endl;
        }

        bool *isThreadRun = new bool(true);

        auto *data = new struct thread_data;
        data->users = users;
        data->rooms = rooms;
        data->client = nullptr;
        data->fd = connFd;
        data->mutex = mutex;
        data->run = isThreadRun;
        auto *clientThread = new pthread_t();
        threads.push_back(clientThread);
        clientThreadIsRun.push_back(isThreadRun);
        pthread_create(clientThread, nullptr, clientConectionHandler, (void *) data);
    }

    for (auto &item : clientThreadIsRun) {
        *item = false;
    }
    for (auto &thread : threads) {
        pthread_join(*thread, NULL);
    }

    close(serverFd);
}



void *serverManagementFunction(void *run) {
    auto *isRun = (bool *) run;
    pthread_detach(pthread_self());
    string command;
    while (true) {
        command.clear();
        cin >> command;
        if (command == "exit") {
            *isRun = false;
            break;
        }
    }
    return nullptr;
}

