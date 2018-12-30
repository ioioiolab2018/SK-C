//
// Created by piotr on 10.12.18.
//

#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <list>
#include "Commons.h"
#include <iterator>
#include <vector>
#include "messageTools.h"
#include "serverStatements.h"
#include "connectionFunctions.h"


// Funkcja obslugująca polaczonego klienta
void *clientConectionHandler(void *data) {
    bool connected = true;
    bool check = false;
    auto *th_data = (struct thread_data *) data;
    int error=0;
    cout << "New client, thread No: " << pthread_self() << endl;

    ssize_t r = 0;
    char buf[BUFF_SIZE];
    string message;
    string lastBuffer;
    bool next = true; // zmienna okreslajaca czy mozna pobrac nastepne dane

    while (true) {
        if(th_data->client!= nullptr){
        if(*th_data->client->fd == -1){
            *th_data->run= false;
        }}

        bzero(buf, BUFF_SIZE + 1);
        if (next) { // przeczytanie wiadomosci od klienta
            r = read(*th_data->fd, buf, BUFF_SIZE);
        }
        if (r > 0 || !next) {
            //zlaczenie otrzymanych danych z poprzednimi nieprzetworzonymi danymi
            message = buf;
            message = lastBuffer.append(message);
            ssize_t start = message.find(START); //odnalezienie poczatku wiadomosci
            if (start == string::npos) {
                next = true;
                lastBuffer = " ";
                continue;
            }
            message.erase(0, static_cast<unsigned long>(start));

            ssize_t end = message.find(END); //odnalezenie konca wiadomosci
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
            if (r <= 0 && errno != EAGAIN) {
                    cout << "Błąd polaczenia z klientem" << endl;
                    error++;
                    if(error > 2){
                        *th_data->run=false;
                    }
                }else {
                    error = 0;
            }
            check = true;
            next= true;
        }



        string reply;
        if (!check) {
            trim_inplace(message); // usuniecie zbednych znakow
            cout << "Wiadomosc: " << message << endl;

            // pobranie komendy (pierwsze "slowo" oddzielone wybranym znakiem)
            string command = get_first_word(message,
                                            DELIMITER);

            if (command == LOGIN) {
                loginFunction(th_data, message);

            } else if (command == LOGOUT) {
                logoutFunction(th_data, connected);

            } else if (command == ENTER) {
                enterFunction(th_data, message);

            } else if (command == MESSAGE) {
                messageFunction(th_data, message);

            } else if (command == CREATE) {
                createRoomFunction(th_data, message);

            } else if (command == ROOM_LIST) {
                roomListFunction(th_data);

            } else if (command == EXIT) {
                exitFunction(th_data);

            } else if (command == USER_LIST) {
                userListFunction(th_data, message);
            }
        }

        // koniec polaczenia spowodowany wylaczeniem serwera
        if (!*th_data->run) {
            cout << "zamykanie watku" << endl;
            next = false;
            logoutFunction(th_data, connected);

            connected = false;;
        }

        //zakonczenie dzialania watku
        if (!connected) {
            close(*th_data->fd);
            cout << "Koniec obslugi klienta" << endl;
            break;
        }
        check = false;
    }
    return nullptr;
}



