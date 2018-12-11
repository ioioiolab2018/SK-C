# ![SK IRC logo](https://github.com/ioioiolab2018/SK-Java/blob/master/src/main/resources/images/logo.png) SK-IRC Server - chat app

Simple chat server created with C++ and Socket.

## Table of contents

-   [General info](#general-info)
-   [Description of the communication protocol](#description-of-the-communication-protocol)
-   [Download](#download)
-   [You may also like](#you-may-also-like)
-   [Build instruction](build-instruction)
## General info

This project is simple IRC chat. It was created for completing the subject of Computer Networks.

## Description of the communication protocol

### Types of messages

-   login - login to the server
-   logout - logout from the server
-   create - create a new room
-   enter - joining an existing room
-   leave - leave the room
-   rooms - request to send a list of rooms
-   users - request to send a list of users in the room
-   message - sending a message

### Examples of messages

| Message sent by the client            | Positive server response                      | Negative server response    |
| ------------------------------------- | --------------------------------------------- | --------------------------- |
| \$login:username#                     | \$login:ok#                                   | \$login:notOk#              |
| \$logout#                             | \$logout:ok#                                  | \$logout:notOk#             |
| \$create:roomName#                    | \$create:ok#                                  | \$create:notOk#             |
| \$enter:roomName#                     | \$enter:ok#                                   | \$enter:notOk#              |
| \$leave:roomName#                     | \$leave:ok#                                   | \$leave:notOk#              |
| \$rooms#                              | \$rooms:roomName;quantity room2Name;quantity# | none                        |
| \$users#                              | \$users:username username2#                   | none                        |
| \$message:useraname;message;sentDate# | none                                          | none                        |
| Sent message                          | \$message:useraname;message;sentDate#         | none                        |

## Download

You can [download](https://github.com/ioioiolab2018/SK-C/releases) the latest installable version of SK-IRC client for Windows, Linux and macOS.

## You may also like

-   [SK-IRC Server](https://github.com/ioioiolab2018/SK-Java) - IRC app for this server.

## Build instruction

- To compile you need Cmake at least 3.10
- To build server run copile.sh, startup program in compiled as serverRun.exec
- Server works only on linux
