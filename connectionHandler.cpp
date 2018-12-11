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

// Funkcja obslugująca polaczonego klienta
void *clientConectionHandler(void *data) {
    bool connected = true;
    bool check = false;
    auto *th_data = (struct thread_data *) data;
    pthread_mutex_t *usersMutex = th_data->mutex[0];
    pthread_mutex_t *roomsMutex = th_data->mutex[1];

    //pthread_detach(pthread_self());
    cout << "New client, thread No: " << pthread_self() << endl;

    ssize_t r = 0;
    char buf[BUFF_SIZE];
    string message;
    string lastBuffer;
    bool next = true; // zmienna okreslajaca czy mozna pobrac nastepne dane

    while (true) {
        bzero(buf, BUFF_SIZE + 1);
        if (next) { // przeczytanie wiadomosci od klienta
            r = read(th_data->fd, buf, BUFF_SIZE);
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
            check = true;
        }


        bool send = false;
        string reply;
        if (!check) {
            trim_inplace(message); // usuniecie zbednych znakow

            cout << "Wiadomosc: " << message << endl;
            string command = get_first_word(message,
                                            DELIMITER); // pobranie komendy (pierwsze "slowo" oddzielone wybranym znakiem)
            send = false; // czy wyslac reply
            // odpowiedz zwrotna do klienta

            //##### LOGIN #####
            if (command == LOGIN) {
                string nick = get_first_word(message, DELIMITER);
                bool ok = true;
                pthread_mutex_lock(usersMutex);
                //sprawdzenie czy istnieje juz uzytkownik z takim nickiem
                for (auto &user : *(th_data->users)) {
                    if (user->nick == nick) {
                        cout << "JEST JUZ TAKI UZYTKOWNIK" << endl;
                        reply = START + LOGIN + DELIMITER + NOT_OK + END;
                        send = true;
                        ok = false;
                        continue;
                    }
                }
                pthread_mutex_unlock(usersMutex);

                // dodanie nowego uzytkownika do listy users
                if (ok) {
                    User *newUser = new User(nick, " ", true);
                    newUser->fd = th_data->fd;
                    th_data->client = newUser;

                    pthread_mutex_lock(usersMutex);
                    th_data->users->push_back(newUser);
                    pthread_mutex_unlock(usersMutex);

                    reply = START + LOGIN + DELIMITER + OK + END;
                    send = true;
                    cout << "Logged: " << nick << endl;
                }


                //##### LOGOUT #####
            } else if (command == LOGOUT) {


                // usuniecie uzytkownika z pokoju w ktorym sie znajdowal
                pthread_mutex_lock(roomsMutex);
                pthread_mutex_lock(usersMutex);
                if (th_data->client->activeRoom != nullptr) {
                    th_data->client->activeRoom->users->remove_if(
                            [th_data](User *u) { return u->nick == th_data->client->nick; });
                    //usuniecie pustyc pokojow
                    if (th_data->client->activeRoom->users->empty()) {
                        th_data->rooms->remove_if(
                                [th_data](Room *r) {
                                    if (r->Name == th_data->client->activeRoom->Name) {
                                        delete (&r);
                                        return true;
                                    } else return false;
                                });
                    }
                }
                // usuniecie uzytkownika z listy users

                th_data->users->remove_if([th_data](User *u) {
                    if (u->nick == th_data->client->nick) {
                        delete (&u);
                        return true;
                    } else
                        return false;
                });

                pthread_mutex_unlock(usersMutex);
                pthread_mutex_unlock(roomsMutex);

                send = true;
                reply = START + LOGOUT + DELIMITER + OK + END;

                connected = false;


                //##### ENTER #####
            } else if (command == ENTER) {

                bool ok = false;
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
                        pthread_mutex_lock(usersMutex);

                        th_data->client->activeRoom = room;
                        pthread_mutex_unlock(usersMutex);

                        reply = START + ENTER + DELIMITER + OK + END;

                        send = true;
                        ok = true;
                        break;
                    }
                }
                pthread_mutex_unlock(roomsMutex);


                // blad, jesli nie ma pokoju o podanej nazwie
                if (!ok) {
                    reply = START + ENTER + DELIMITER + NOT_OK + END;
                    send = true;
                }

                //##### MESSAGE #####
            } else if (command == MESSAGE) {
                //przesylanie wiadomosci do wszytskich uzykownikow ktorzy sa w tym sammy pokoju
                if (th_data->client->activeRoom != nullptr) {
                    message = get_first_word(message, DELIMITER);
                    reply = START + MESSAGE + DELIMITER + message + END;

                    pthread_mutex_lock(roomsMutex);
                    for (auto &user: *(th_data->client->activeRoom->users)) {
                        if (user->logged && user->nick != th_data->client->nick) {
                            cout << reply << endl;
                            ssize_t n = write(user->fd, reply.c_str(), reply.size());
                        }
                    }
                    pthread_mutex_unlock(roomsMutex);
                }

                //##### CREATE #####
            } else if (command == CREATE) {
                bool ok = true;
                string name = get_first_word(message, DELIMITER);
                pthread_mutex_lock(roomsMutex);
                for (auto &room : *(th_data->rooms)) {
                    if (room->Name == name) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    th_data->rooms->push_back(new Room(name, " "));
                    reply = START + CREATE + DELIMITER + OK + END;
                    send = true;
                } else {
                    reply = START + CREATE + DELIMITER + NOT_OK + END;
                    send = true;
                }
                pthread_mutex_unlock(roomsMutex);


                //##### ROOM LIST #####
            } else if (command == ROOM_LIST) {
                send = true;
                reply = START + ROOM_LIST + DELIMITER;

                pthread_mutex_lock(roomsMutex);
                for (auto &room : *(th_data->rooms)) {
                    reply += room->Name + ";" + to_string(room->users->size()) + " ";
                }
                pthread_mutex_unlock(roomsMutex);

                reply += END;


                //##### EXIT #####
            } else if (command == EXIT) {
                // usuniecie uzytkownika z pokoju w ktorym sie znajdowal
                pthread_mutex_lock(roomsMutex);
                if (th_data->client->activeRoom != nullptr) {
                    th_data->client->activeRoom->users->remove_if(
                            [th_data](User *u) { return u->nick == th_data->client->nick; });

                    //usuniecie pustyc pokojow
                    if (th_data->client->activeRoom->users->empty()) {
                        th_data->rooms->remove_if(
                                [th_data](Room *r) { return r->Name == th_data->client->activeRoom->Name; });
                    }
                    th_data->client->activeRoom = nullptr;
                }
                pthread_mutex_unlock(roomsMutex);
                reply = START + EXIT + DELIMITER + OK + END;
                send = true;


                //##### USER LIST #####
            } else if (command == USER_LIST) {
                string name = get_first_word(message, DELIMITER);
                send = true;
                reply = START + USER_LIST + DELIMITER;

                pthread_mutex_lock(roomsMutex);
                for (auto &room : *(th_data->rooms)) {
                    if (room->Name == name) {
                        for (auto &user : *(room->users)) {
                            reply += user->nick + " ";
                        }
                    }
                }
                pthread_mutex_unlock(roomsMutex);

                reply += END;
            }
        }

        // koniec polaczenia spowodowany wylaczeniem serwera
        if (!*th_data->run) {
            cout << "zamykanie watku" << endl;
            next = false;
            // usuniecie uzytkownika z pokoju w ktorym sie znajdowal
            pthread_mutex_lock(roomsMutex);
            pthread_mutex_lock(usersMutex);
            if (th_data->client->activeRoom != nullptr) {
                th_data->client->activeRoom->users->remove_if(
                        [th_data](User *u) { return u->nick == th_data->client->nick; });
                //usuniecie pustych pokojow
                if (th_data->client->activeRoom->users->empty()) {
                    th_data->rooms->remove_if(
                            [th_data](Room *r) {
                                if (r->Name == th_data->client->activeRoom->Name) {
                                    delete (&r);
                                    return true;
                                } else return false;
                            });
                }
            }
            // usuniecie uzytkownika z listy users

            th_data->users->remove_if([th_data](User *u) {
                if (u->nick == th_data->client->nick) {
                    delete (&u);
                    return true;
                } else
                    return false;
            });

            pthread_mutex_unlock(usersMutex);
            pthread_mutex_unlock(roomsMutex);

            send = true;
            reply = START + LOGOUT + DELIMITER + OK + END;

            connected = false;
        }

        // wyslanie klientowi odpowiedzi
        if (send) { ;
            cout << reply << endl;
            ssize_t n = write(th_data->fd, reply.c_str(), reply.size());
            send = false;
        }

        //zakonczenie dzialania watku
        if (!connected) {
            close(th_data->fd);
            cout << "Koniec obslugi klienta" << endl;
            break;
        }
        check = false;
    }
    return nullptr;
}
