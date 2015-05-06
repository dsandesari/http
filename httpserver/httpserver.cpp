
#include <cerrno>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <future>
#include <vector>
#include <dirent.h>
#include <ctime>

#include <unistd.h>
#include <sys/socket.h>  // Socket interface
#include <arpa/inet.h>   // Utility network functions
#include <netinet/in.h>  // Internet protocol interface
// Manages the coniguration of the socket.
struct Config
{
  int network;    // Specifies the network layer.
  int transport;  // Selects a transport profile.
  int port;       // The application port number.
  char * rootdir; // The root directory of server.
};

bool parse_command_line(int argc, char** argv, Config&);
void run_stream_server(int sd, Config&);
char * getContentType(char * file);

int
usage()
{
  std::cout << "usage: httpserver <port> <path/to/rootdir>\n\n";
  return 0;
}

int
main(int argc, char* argv[])
{

  // Parse the command line arguments.
  Config cfg;
  cfg.network=AF_INET;//AF_INET Network Layer Protocol (ipv4)
  cfg.transport=SOCK_STREAM;  //SOCK_STREAM Transport Layer Protocol (tcp)
  if (!parse_command_line(argc, argv, cfg)) {
    usage();
    return -1;
  }

  // Create the socket.
  int sd = socket(cfg.network, cfg.transport, 0);
  if (sd < 0) {
    std::cout << "error (socket): " << std::strerror(errno) << '\n';
    return -1;
  }

  // Bind the socket to the local address. We need
  // to set up a socket address for this operation.
   

    // Initialize the IPv4 address.
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); // Zero-initialize the address
    addr.sin_family = AF_INET;           // Set the address family
    addr.sin_addr.s_addr = INADDR_ANY;   // Allow messages from anywhere
    addr.sin_port = htons(cfg.port);     // Set the port number

    // Bind the socket to the address.
    if (bind (sd, (sockaddr*)&addr, sizeof(addr)) < 0) {
      std::cout << "error (bind): " << std::strerror(errno) << '\n';
      return -1;
    }
  

  // Run a server based on the requested transport type i.e TCP.
    run_stream_server(sd,cfg);


  // Close the socket and exit.
    shutdown(sd, SHUT_RDWR);
    close(sd);
}

bool
parse_command_line(int argc, char** argv, Config& cfg)
{
  if (argc != 3) {
    std::cout << "error (config): wrong number of command line arguments\n";
    return false;
  }

// Get the port number.
  std::string port = argv[1];
  std::stringstream ss(port);
  ss >> cfg.port;
  if (!ss) {
    std::cout << "error (config): invalid port number\n";
    return false;
  }
  //Get the root directory
  DIR * dirptr;
  cfg.rootdir = argv[2];
  if ((dirptr = opendir(cfg.rootdir)) == NULL) {
      printf("error (config): invalid path to root directory\n");
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

char* 
getContentType(char * file) 
{
 char * temp;
 if ((temp = strstr(file, ".html")) != NULL) {
    return ((char*)"text/html");
 } 
 else if ((temp = strstr(file, ".txt")) != NULL) {
    return ((char*)"text/html");
 } 
 else 
    return nullptr;
}

void
run_stream_server(int sd,Config& cfg) 
{
  /*
   This is a very simple concurrent http transfer
   server that can handle requests from multiple 
   simultaneous clients.
  */


  // Start listening for connections. SOMAXCONN is the largest
  // possible backlog of unserviced requests on Linux.
 std::cout << "**** Server up and running... "  << '\n';
 if (listen(sd, SOMAXCONN) < 0) {
    std::cout << "error (listen): " << std::strerror(errno) << '\n';
    return;
  }
  while(1){  
     /* Accepts a connection and then creates a child to do the work 
        After the child proc is done  it loop backs and waits 
        for a another connection
      */
    sockaddr_storage addr;           // Stores the address of the client.
    socklen_t length = sizeof(addr); // Length of the stored address.

    // Accept the next queued connection. Note that this will
    // block (stop the program) if no connections are queued.
    // If connections are available, this returns immediately.
    //
    // The value cd (client descriptor) is a socket descriptor
    // for the connected client.
    std::cout << "**** Accepting incoming connections on port: "  <<cfg.port<< '\n';
    int cd = accept (sd, (sockaddr*)&addr, &length);
    if (cd < 0) {
      std::cout << "error (accept): " << std::strerror(errno) << '\n';
    }
    int pid;
    if ( (pid=fork()) == 0) {         /* Child process starts here. */

      close(sd);
   
      std::string name = get_address_name(addr);
      constexpr int bufsize = 1 << 16;
      std::cout << '[' << name << ']' << " connected\n";
      char buf[bufsize]; // Stores the messages
      char databuf[bufsize];
      // Block until there is data to be recieved. This returns
      // the number of bytes available in the messge queue.
      int n = recv(cd, buf, bufsize, 0);
      if (n <= 0) {
        if (n == 0) {
          std::cout<<"SHUTDOWN\n";
          shutdown(cd, SHUT_RDWR);
          close(cd);
        } else {
          std::cout << "error (recv): " << std::strerror(errno) << '\n';
        }
        return ;
      }
      //Processing the incoming request
      char *requestedPath = (char*)malloc(bufsize*sizeof(char));
      char *actualPath = (char*)malloc(bufsize*sizeof(char));
      char *httpHeader = (char*)malloc(bufsize*sizeof(char));
      char requestType[4];// GET or HEAD
      char httpVersion[8];// HTTP/1.0 
      sscanf(buf, "%s %s %s", requestType, requestedPath, httpVersion);
      std::cout << '[' << name << ']' << " received: " << buf;
      requestedPath++;//Ignores the slash
      sprintf(actualPath,"%s/%s", cfg.rootdir, requestedPath);
      std::time_t result = std::time(NULL);
      //Sends response to client
      FILE * fileptr;char *contentType;
      memset(&buf, 0, sizeof(buf));
      std::cout << '[' << name << ']' << " sending http response.. "<< '\n';
      //If file is not found or content type is not text/html
      if ((fileptr = fopen(actualPath, "r")) == NULL|(contentType = getContentType(requestedPath))==NULL ) {
          strcpy(buf,"HTTP/1.0 404 Not Found\n");
          send(cd, buf, sizeof(buf), 0);

      } else {
          int contentLength=0,c;
          while ((c = getc(fileptr)) != EOF) {contentLength++;}
          std::stringstream ss;
          ss << contentLength;
          std::string str = ss.str();
          strcpy(buf,"HTTP/1.0 200 OK\n");
          sprintf(httpHeader, "Date: %sServer: %s:%d\nContent-Type:%s\nContent-Length:%s\n\n", std::asctime(std::localtime(&result)), "httpserver", cfg.port, contentType, str.c_str());
          strcat(buf,httpHeader);
          send(cd, buf, sizeof(buf), 0);
          memset(&buf, 0, sizeof(buf));
         //Sends contents of the file
          if(strcmp(requestType,"HEAD")){
            memset(&databuf, 0, sizeof(databuf));
            rewind(fileptr);
            while (!feof(fileptr)) { 
              fread(&databuf, sizeof(databuf), 1, fileptr);
              send(cd, databuf, sizeof(databuf), 0);
              memset(&databuf, 0, sizeof(databuf));
            }
          }

      }
      std::cout << '[' << name << ']' << " http response sent"<< '\n';
      close(cd);
      exit(0);
    }    /* Parent continues from here */
    close(cd);
  }//end while
}


//   ./httpserver 7615 /Users/ROHITH/http/httpserver/rootdir

