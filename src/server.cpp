#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  std::cout << "Starting the server ...." << std::endl;

  /**
   * Create a socket.
   *
   * The `socket()` system call is used to create a new socket. It takes three arguments:
   *  - The address family (such as `AF_INET` for IPv4, or `AF_INET6` for IPv6)
   *  - The socket type (such as `SOCK_STREAM` for TCP, or `SOCK_DGRAM` for UDP)
   *  - The protocol (usually set to 0 to automatically choose the right protocol based on the
   *    address family and socket type)
   *
   * The `socket()` system call returns a file descriptor (a small integer). If the call fails,
   * it returns -1.
   *
   * @example
   * `int socket(int domain, int type, int protocol);`
   */
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Error creating the socket !" << std::endl;
    return 1;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family      = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available interface[0, 0, 0, 0]
  serverAddr.sin_port        = htons(4000); // Bind to port 4000

  /** The `bind()` system call is used to bind a socket to a particular address and port. It
   * takes three arguments:
   *
   *  - The file descriptor of the socket to bind (returned by `socket()`)
   *  - A pointer to a `sockaddr` structure that contains the local address and port
   *  - The size of the `sockaddr` structure
   *
   * @example
   * `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
   */
  if (bind(server_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    std::cerr << "Error binding the socket !" << std::endl;
    close(server_socket);
    return 1;
  }

  /**
   * The listen() system call is used to put a socket into the listening state. This allows it
   * to accept incoming connections from clients. It takes two arguments:
   * - The `server_socket` file descriptor (returned by `socket()`)
   * - The maximum number of pending connections that can be queued up before connections are
   * refused
   */
  listen(server_socket, 1);

  /**
   * The accept() system call is used to accept a new incoming connection. It takes three
   * arguments:
   * - The `server_socket` file descriptor (returned by `socket()`)
   * - A pointer to a `sockaddr` structure that will store the address of the client
   * - A pointer to an integer that will store the size of the `sockaddr` structure
   *
   * The accept() system call returns a new file descriptor that can be used to communicate
   * with the client. If the call fails, it returns -1.
   */
  int  client_socket = accept(server_socket, NULL, NULL);
  char buff[1024];
  while (true) {
    memset(buff, 0, sizeof(buff));
    recv(client_socket, buff, sizeof(buff), 0);
    std::cout << "Message from client: " << buff;
    if (strcmp(buff, "exit\n") == 0)
      break;
  }
  close(server_socket);
  return (0);
}