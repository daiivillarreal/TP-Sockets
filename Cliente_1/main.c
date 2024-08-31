#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <locale.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    char buffer[256];
    int option, longitud;
    setlocale(LC_CTYPE, "");

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Fallo al iniciar Winsock.\n");
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear socket.\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8888);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Error de conexion.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (1) {
        printf("Elige una opción:\n\n");
        printf("1. Generar usuario\n");
        printf("2. Generar Contraseña\n");
        printf("0. Salir\n");
        printf("\nOpción ingresada: ");
        scanf("%d", &option);

        if (option == 0) {
            break;
        }

        printf("Ingrese longitud: ");
        scanf("%d", &longitud);

        // Validación de longitud para evitar enviar valores no válidos
        if (longitud < 1) {
            printf("Longitud inválida.\n");
            continue;
        }

        // Formato del mensaje enviado al servidor
        snprintf(buffer, sizeof(buffer), "%d %d", option, longitud);

        // Enviar mensaje al servidor
        if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            printf("Error al enviar datos.\n");
            break;
        }

        // Limpiar el buffer antes de recibir datos
        memset(buffer, 0, sizeof(buffer));
        int recvLen = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (recvLen == SOCKET_ERROR) {
            printf("Error al recibir datos.\n");
            break;
        } else if (recvLen == 0) {
            printf("El servidor cerró la conexión.\n");
            break;
        }

        buffer[recvLen] = '\0'; // Null-terminar el buffer
        printf("\nRespuesta: %s\n", buffer);
        printf("Presione una tecla para volver al menú...\n");
        getch();
        system("cls");
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
