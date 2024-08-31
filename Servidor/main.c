#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <locale.h>
#include <ctype.h>

#pragma comment(lib, "ws2_32.lib")

void crearUsuario(int len, char* usuario);
void crearClave(int len, char* clave);

char traerCaracter(const char *cadena) {
    size_t cant = strlen(cadena);
    char caracter = cadena[rand() % cant];
    return caracter;
}

void crearUsuario(int len, char* usuario) {
    const char *vocales = "aeiou";
    const char *consonantes = "bcdfghjklmnpqrstvwxyz";

    if (len >= 5 && len <= 15) {
        int vocal = rand() % 2; // 0 o 1 para la primera posición

        for (int i = 0; i < len; i++) {
            if (vocal) {
                usuario[i] = traerCaracter(vocales);
                vocal = 0;
            } else {
                usuario[i] = traerCaracter(consonantes);
                vocal = 1;
            }
        }
        usuario[len] = '\0';
    } else {
        strcpy(usuario, "Error: Longitud fuera de rango");
    }
}

void crearClave(int len, char* clave) {
    const char *caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    if (len >= 8 && len < 50) {
        for (int i = 0; i < len; i++) {
            clave[i] = traerCaracter(caracteres);
        }
        clave[len] = '\0';
    } else {
        strcpy(clave, "Error: Longitud fuera de rango");
    }
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, cliente;
    int longitud_cliente;
    char buffer[256];
    int recvLen;

    srand((unsigned int)time(NULL));
    setlocale(LC_CTYPE, "");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Fallo al iniciar Winsock.\n");
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("No se pudo crear socket.\n");
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error al realizar el bind\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 5);

    printf("Esperando conexiones entrantes...\n");
    longitud_cliente = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr*)&cliente, &longitud_cliente);
    if (clientSocket == INVALID_SOCKET) {
        printf("Fallo al aceptar la conexion\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    while (1) {
        recvLen = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (recvLen == SOCKET_ERROR) {
            printf("Error al recibir datos.\n");
            break;
        }
        buffer[recvLen] = '\0';

        char response[256];
        int option = buffer[0] - '0';
        int len = atoi(buffer + 2);

        if (option == 1) {
            crearUsuario(len, response);
        } else if (option == 2) {
            crearClave(len, response);
        } else {
            strcpy(response, "Error: Opción inválida");
        }

        send(clientSocket, response, strlen(response), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
