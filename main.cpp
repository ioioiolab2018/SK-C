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

    //bind socket
    if (bind(serverFd, (struct sockaddr *) &svrAdd, sizeof(svrAdd)) < 0) {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(serverFd, 20);

    len = sizeof(clntAdd);

    //liczba klientow/wątków
    pthread_t serverThread;
    pthread_create(&serverThread, nullptr, serverManagementFunction, (void *) &run);


    while (run) {
        cout << "Listening" << endl;
        //this is where client connects. svr will hang in this mode until client conn
        int connFd = accept(serverFd, (struct sockaddr *) &clntAdd, &len);

        if (connFd < 0) {
            cerr << "Cannot accept connection" << endl;
            return 0;
        } else {
            cout << "Connection successful" << endl;
        }

        auto *data = new struct thread_data;
        data->users = users;
        data->rooms = rooms;
        data->client = nullptr;
        data->fd = connFd;
        data->mutex = mutex;

        auto *clientThread = new pthread_t();
        threads.push_back(clientThread);
        pthread_create(clientThread, nullptr, clientConectionHandler, (void *) data);
    }
}


void *serverManagementFunction(void *run) {
    auto *isRun = (bool *) run;
    cout << "zamyaknie dziala" << endl;
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

