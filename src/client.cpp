#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  std::cout << "Starting the Client ..." << std::endl;

  /**
   * Create a socket.
   *
   * The `socket()` system call creates a new socket and returns a file descriptor for it. It
   * takes three arguments:
   *  - The address family (e.g., `AF_INET` for IPv4)
   *  - The socket type (e.g., `SOCK_STREAM` for TCP)
   *  - The protocol (typically set to 0 to use the default protocol for the given socket type)
   *
   * If the socket creation fails, `socket()` returns -1.
   *
   * @example
   * `int socket(int domain, int type, int protocol);`
   */
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    std::cerr << "Error creating the socket ..." << std::endl;
    return 1;
  }

  /**
   * Define the server address to connect to.
   *
   * A `sockaddr_in` structure is used to specify the address and port of the server. The
   * `sin_family` is set to `AF_INET` to indicate IPv4. The `sin_port` is set to 4000, using
   * `htons()` to ensure the correct byte order. The `sin_addr.s_addr` is set to `INADDR_ANY`,
   * which is typically used for binding but here is effectively the loopback address
   * (`127.0.0.1`) for connecting to the local machine.
   */
  sockaddr_in clientAddr;
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port   = htons(4000); // Set port to 4000, converting to network byte order
  clientAddr.sin_addr.s_addr = INADDR_ANY; // Set IP address to any local address (127.0.0.1)

  /**
   * The `connect()` system call is used to initiate a connection to the server.
   * It takes three arguments:
   *  - The socket file descriptor (returned by `socket()`)
   *  - A pointer to the `sockaddr` structure representing the server address
   *  - The size of the `sockaddr` structure
   *
   * If the connection fails, the program will not proceed to send messages.
   *
   * @example
   * `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
   */
  if (connect(client_socket, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
    std::cerr << "Error connecting to the server ..." << std::endl;
    close(client_socket);
    return 1;
  }

  /**
   * The client is now connected to the server and can send messages.
   *
   * A loop is used to continuously prompt the user to enter messages. The `fgets()` function
   * reads a line from the standard input (`stdin`) and stores it in the `mess` buffer.
   *
   * - `send()` sends the message to the server. It takes four arguments:
   *   - The socket file descriptor
   *   - A pointer to the buffer containing the message
   *   - The length of the message
   *   - Flags (set to 0 for no special behavior)
   *
   * If the user inputs "exit", the client will break out of the loop and terminate the
   * connection.
   */
  char mess[1024];
  while (true) {
    std::cout << "Enter the message to send: ";
    fgets(mess, sizeof(mess), stdin); // Read input from the user

    send(client_socket, mess, strlen(mess), 0); // Send the message to the server

    if (strcmp(mess, "exit\n") == 0) // Check if the user wants to exit
      break;

    memset(mess, 0, sizeof(mess)); // Clear the message buffer for the next input
  }

  /**
   * Close the socket.
   *
   * The `close()` system call is used to close the client socket and release the associated
   * resources. It takes one argument:
   * - The file descriptor of the socket to be closed.
   */
  close(client_socket);
  return 0;
}
