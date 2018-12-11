//
// Created by piotr on 11.12.18.
//

#ifndef SK_CONNECTIONFUNCTIONS_H
#define SK_CONNECTIONFUNCTIONS_H

#include "Commons.h"
#include <pthread.h>
#include <string>

using namespace std;

void sendToClient(int * fd, thread_data *th_data, string &reply); // wyslanie  wiadomosci z reply do  uzytkownika

void createRoomFunction(thread_data *th_data, string &message); // dodanie nowego pokoju

void roomListFunction(thread_data *th_data); // wysłanie listy pokojow do uzytkownika z th_data (client)

void userListFunction(thread_data *th_data,
                      string &message); // wysłanie listy uzytkownikow w danym pokoju do uzytkownika z th_data (client)

void logoutFunction(thread_data *th_data,
                    bool &connected); // wylogowanie (wyjscie z pokoju i usuniecie z listy użytkownikow)

void exitFunction(thread_data *th_data); // wyjscie uzytkownika z pokoju, pokój sie usuwa jezeli zosatnie pusty

void loginFunction(thread_data *th_data, string &message); // zalogowanie nowego uzytkownika

void enterFunction(thread_data *th_data, string &message); // wejscie uzytkownika do pokoju

void
messageFunction(thread_data *th_data, string &message); // rozeslanie wiadomosci do uzytkownikow z tego samego pokoju


#endif //SK_CONNECTIONFUNCTIONS_H
