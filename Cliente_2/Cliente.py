import socket

def main():
    server_ip = '127.0.0.1'
    server_port = 8888

    # Crear un socket TCP/IP
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Conectar al servidor
        client_socket.connect((server_ip, server_port))
        print("Conectado al servidor en {}:{}".format(server_ip, server_port))

        while True:
            # Mostrar opciones al usuario
            print("\nElige una opción:")
            print("1. Generar usuario")
            print("2. Generar Contraseña")
            print("0. Salir")
            option = input("Opción ingresada: ")

            if option == '0':
                break

            longitud = input("Ingrese longitud: ")

            try:
                longitud = int(longitud)
                if longitud < 1:
                    print("Longitud inválida.")
                    continue
            except ValueError:
                print("Por favor ingrese un número válido para la longitud.")
                continue

            # Crear mensaje para enviar al servidor
            message = "{} {}".format(option, longitud)

            # Enviar el mensaje al servidor
            client_socket.sendall(message.encode('utf-8'))

            # Recibir respuesta del servidor
            response = client_socket.recv(256).decode('utf-8').strip()
            print("Respuesta: {}".format(response))

    except Exception as e:
        print("Error: {}".format(e))
    
    finally:
        # Cerrar el socket
        client_socket.close()
        print("Conexión cerrada.")

if __name__ == '__main__':
    main()
