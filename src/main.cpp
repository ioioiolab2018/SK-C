#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <list>

#include "Commons.h"
#include "connectionHandler.h"
#include "serverStatements.h"

using namespace std;

void *serverManagementFunction(void *run); //  funkcja do zamykania serwera

// Główna funckja uruchamiajaca serwer
int main(int argc, char *argv[]) {
    int port, serverFd;

    // Mutexy
    pthread_mutex_t usersMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t roomsMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t connectionMutex = PTHREAD_MUTEX_INITIALIZER; // nie używany
    pthread_mutex_t *mutex[3] = {&usersMutex, &roomsMutex, &connectionMutex};

    socklen_t len; // długosc adresu
    bool run = true; // zmienna okreslajca czy serwer działa

    auto *users = new list<User *>; // lista uzytkownikow
    auto *rooms = new list<Room *>; // lista pokojow
    startRooms(rooms);

    struct sockaddr_in svrAdd{};
    struct sockaddr_in clntAdd{};

    list<pthread_t *> threads; // lista wątków

    port = PORT;

    if (argc >= 2 && !((port > 65535) || (port < 2000))) {
        port = atoi(argv[1]);
    }
    cout << "*** SERWER  IRC TCP ***" << endl << endl;
    cout << "Wybrany port: " + to_string(port) << endl;
    cout << "Wpisz \"exit\" aby zamknac serwer" << endl;

    // utworzenie socketu
    serverFd = socket(AF_INET, SOCK_STREAM, 0);;

    if (serverFd < 0) {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    bzero((char *) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    svrAdd.sin_port = htons(static_cast<uint16_t>(port));

    // Zmiane ustwaien socket, dodanie timeout
    struct timeval tv{};
    tv.tv_sec = 1; // czas w sec
    tv.tv_usec = 0;
    setsockopt(serverFd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    //bind socket
    if (bind(serverFd, (struct sockaddr *) &svrAdd, sizeof(svrAdd)) < 0) {
        cerr << "Cannot bind" << endl;
        return 0;
    }

    listen(serverFd, 20);

    len = sizeof(clntAdd);

    // utworzenie watku wylaczajacego serwer
    pthread_t serverThread;
    pthread_create(&serverThread, nullptr, serverManagementFunction, (void *) &run);

    list<bool *> clientThreadIsRun;
    cout << "Listening" << endl;
    int error=0;
    while (run) {
        // oczekiwanie na polaczenie (wlaczony timeout, operacja blokuje na 1s)


        int connFd = accept(serverFd, (struct sockaddr *) &clntAdd, &len);
        if (connFd < 0) {
            error++;
            if( error > 2 && errno != EAGAIN){ // wylaczenie serwera po błędzie innym n
                cout << "Awaria, serwer zostanie wylaczony" << endl;
                run= false;
            }else {
                error = 0;
            }
            continue;
        } else {
            cout << "Connection successful" << endl;


        bool *isThreadRun = new bool(true);

        auto *data = new struct thread_data; // stworzenie danych nowego watku
        data->users = users;
        data->rooms = rooms;
        data->client = nullptr;
        data->fd = new int(connFd);
        data->mutex = mutex;
        data->run = isThreadRun;
        auto *clientThread = new pthread_t();
        threads.push_back(clientThread);
        clientThreadIsRun.push_back(isThreadRun);
        // stworzenie nowego watku
        pthread_create(clientThread, nullptr, clientConectionHandler, (void *) data);
        }
        error=0;
    }

    // wylaczenie klientow
    for (auto &item : clientThreadIsRun) {
        *item = false;
    }
    for (auto &thread : threads) {
        pthread_join(*thread, nullptr);
        delete (thread);
    }
    // czyszczenie
    threads.clear();
    users->clear();
    delete (users);
    rooms->clear();
    delete (rooms);
    clientThreadIsRun.clear();
    close(serverFd);
    return 0;
}


// Zamykanie serwera, wykonywane w watku serverThread
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

