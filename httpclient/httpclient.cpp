
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <sys/types.h>   // Various system types
#include <sys/socket.h>  // Socket interface
#include <arpa/inet.h>   // Utility network functions
#include <netinet/in.h>  // Internet protocol interface
#include <netdb.h>       // DNS lookup

// Manages the coniguration of the socket.
struct Config
{
  int         network;    // Specifies the network layer.
  int         transport;  // Selects a transport profile.
  int         port;       // The application port number.
  const char* host;       // The DNS name of the host.
};
struct HttpRequest
{
  char * requestType; 
  char * requestedPath;
  char * httpVersion; 
};

bool parse_command_line(int argc, char** argv, HttpRequest&, Config&);
void run_stream_client(int cd, const Config&, const HttpRequest&);

int
usage()
{
  std::cout << "usage: httpclient <GET/HEAD> <path/to/filename.html> <hostname> <port> <HTTP/1.0>\n\n";
  return 0;
}

int
main(int argc, char* argv[])
{
  // Parse the command line arguments.
  Config cfg;
  cfg.network=AF_INET;        //AF_INET Network Layer Protocol (ipv4)
  cfg.transport=SOCK_STREAM;  //SOCK_STREAM Transport Layer Protocol (tcp)
  HttpRequest htr;
  if (!parse_command_line(argc, argv, htr, cfg)) {
    usage();
    return -1;
  }
 
  // Create the socket.
  int sd = socket(cfg.network, cfg.transport, 0);
  if (sd < 0) {
    std::cout << "error: " << std::strerror(errno) << '\n';
    return -1;
  }
  
  // Run a server based on the requested transport type: TCP.
  run_stream_client(sd, cfg, htr);

 
  //Close the socket and exit.
  shutdown(sd, SHUT_RDWR);
  close(sd);

}

bool
parse_command_line(int argc, char** argv, HttpRequest& htr, Config& cfg)
{
  if (argc != 6) {
    std::cout << "error (config): wrong number of command line arguments\n";
    return false;
  }

  // Request type is either GET/HEAD.
  htr.requestType= argv[1];
  if (strcmp(htr.requestType, "GET")!=0 && strcmp(htr.requestType, "HEAD")!=0) {
    std::cout << "error (config): Request type must be 'HEAD' or 'GET'\n";
    return false;
  }
  htr.requestedPath=argv[2];
  cfg.host=argv[3];
// Get the port number.
  std::string port = argv[4];
  std::stringstream ss(port);
  ss >> cfg.port;
  if (!ss) {
    std::cout << "error (config): invalid port number\n";
    return false;
  }

  // Get the http version.
  htr.httpVersion = argv[5];
  if (strcmp(htr.httpVersion, "HTTP/1.0")!=0) {
    std::cout << "error (config): Only Http Version 1.0 Supported\n";
    return false;
  }

  return true;
}


// Returns a string representation of the given address.
std::string
get_address_name(sockaddr_storage& addr)
{
  // Get the address field from the connected end point.
  void* ptr;     // Points to the actual address field
  if (addr.ss_family == AF_INET) {
    sockaddr_in* p = (sockaddr_in*)&addr;
    ptr = &p->sin_addr;
  } else {
    std::cout << addr.ss_family << '\n';
    return "<unknown>";
  }
  
  // Get a textual representaiton of the client's address.
  char name[64]; // Will store the address string
  if (!inet_ntop(addr.ss_family, ptr, name, 64))
    return "<error>";
  else
    return name;
}

void
run_stream_client (int sd, const Config& cfg,const HttpRequest& htr)
{
  // Give hints about the connection being requested.
  addrinfo hint {};
  std::memset(&hint, 0, sizeof(hint));
  hint.ai_family = cfg.network;     // Use the requsted network
  hint.ai_socktype = cfg.transport; // Use the requested transport

  // TODO: We could pass a string representation of the
  // port number getaddrinfo (or a well-known service name).
  addrinfo* info;
  int result = getaddrinfo(cfg.host, NULL, &hint, &info);
  if (result < 0) {
    std::cout << "error (getaddrinfo):" << gai_strerror(result) << '\n';
    return;
  }

  // Print out the connected address name.
  sockaddr_storage store;
  std::memcpy(&store, info->ai_addr, info->ai_addrlen);
  

  // Connect to the remove server.
  // Bind the socket to the local address. We need
  // to set up a socket address for this operation.

    // Initialize the IPv4 address.
    sockaddr_in* addr = (sockaddr_in*)info->ai_addr;
    addr->sin_port = htons(cfg.port);

    // Bind the socket to the address.
    if (connect (sd, info->ai_addr, info->ai_addrlen) < 0) {
      std::cout << "error (connect ipv4): " << std::strerror(errno) << '\n';
      return;
    }
    std::cout << "** connected to: " << get_address_name(store) << '\n';
      // Send input to the server.
    constexpr int bufsize = 1 << 16;  // Max buffer size
    char buf[bufsize];                // Stores the messages
    memset(&buf, 0, sizeof(buf));
    sprintf(buf, "%s %s %s\n\n", htr.requestType, htr.requestedPath, htr.httpVersion);

  // Send data to the connected client. 
    int n = send(sd, buf, bufsize, 0);
    if (n == 0) {
        std::cout<<"SHUTDOWN\n";
        shutdown(sd, SHUT_RDWR);
        close(sd);
    } 
    else if(n < 0){
        std::cout << "error (send): " << std::strerror(errno) << '\n';
        return;
    }
    std::cout << "** loading content: " <<'\n';
   //Receive from server
    int r;
    memset(&buf, 0, sizeof(buf));
    while((r= recv(sd, buf, bufsize, 0)>0)){
      std::cout<<buf<<"\n"; 
      memset(&buf, 0, sizeof(buf));
    }
   if(r<0){
      std::cout<<"Error receiving HTTP status!\n";
     }
   freeaddrinfo(info);
}
// ./httpclient GET /index.html localhost 7777 HTTP/1.0


