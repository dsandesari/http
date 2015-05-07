#HTTP Client and Server

Deekshith Sandesari  
Department Of Computer Science  
The University Of Akron  
E-mail: ds168@zips.uakron.edu


 
##Abstract

Telecommunication systems follow a set of rules called protocols to enable effective transmission or exchange of information between two or more communication devices by means of any physical medium. Network models such as TCP/IP and OSI provide as set of design guidelines for data communication systems and also describe how various layers of data communication systems interact. Systems use a set of cooperating protocols instead of single protocol. HTTP is one such protocol originally implemented to transfer hypertext over the World Wide Web. This project aims in developing a simple HTTP client and a HTTP server application.

Keywords – HTTP (HyperText Transfer Protocol), TCP (Transmission Control Protocol), IP (Internet Protocol), OSI (Open Systems Interconnection), HTML (Hyper Text Markup Language)

 
##1.HTTP Overview (Overview of the Problem)

The Hypertext Transfer Protocol (HTTP) is an application-level protocol for distributed, collaborative, hypermedia information systems. This is the foundation for data communication for the World Wide Web (i.e. internet) since 1990. HTTP is a generic and stateless protocol that can be used for other purposes as well using extensions of its request methods, error codes, and headers. 


History:  
  
  Tim Berners-Lee and his team at CERN are credited with inventing the original HTTP along with HTML and the associated technology for a web server and a text-based web browser. Berners-Lee first proposed the "World Wide Web" project in 1989. The first version of the protocol had only one method, namely GET, which would request a page from a server. The response from the server was always an HTML page.
The first documented version of HTTP was HTTP V0.9 (1991). RFC 2616 (June 1999) defines HTTP/1.1 is most commonly used today. HTTP/2 is currently in draft form. This project uses HTTP V1.0 (1996) specified in RFC 1945.

Overall Operation:  
  
  The HTTP protocol is based on a request/response paradigm as shown in Figure 1.1. A client establishes a connection with a server and sends a request to the server in the form of a request method, URI, and protocol version, followed by a MIME-like message containing request modifiers, client information, and possible body content. The server responds with a status line, including the message's protocol version and a success or error code, followed by a MIME-like message containing server information, entity meta-information, and possible body content.  
	![Alt text](http://www.tankonyvtar.hu/en/tartalom/tamop425/0027_ADW1/images/ADW100.png )  
	    Figure 1.1 HTTP Client Server
		

##2. HTTP Messages (Messages Sent)

	HTTP messages consist of requests from client to server and responses from server to client as mentioned below:   
	
	    HTTP-message     =   Simple-Request		 	HTTP/0.9 messages
   				           |    Simple-Response	

	    Simple-Request   =     HTTP Method
					Zero or more header fields followed by CRLF
					An empty line indicating the end of the header fields
					Optionally a message-body

		Example:		GET /hello.html HTTP/1.0
					User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3 
					Host: www.example.com 
					Accept-Language: en, mi 
					Date: Mon, 27 Jul 2009 12:28:53 GMT 
					Server: Apache
					Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT 
					ETag: "34aa387-d-1568eb00" 
					Accept-Ranges: bytes 
					Content-Length: 51 
					Vary: Accept-Encoding 
					Content-Type: text/plain



		Simple-Response  = 	Status Message
					Zero or more header fields followed by CRLF
					An empty line indicating the end of the header fields
					Optionally a message-body

		Example:		HTTP/1.1 200 OK
					Date: Mon, 27 Jul 2009 12:28:53 GMT
					Server: Apache/2.2.14 (Win32)
					Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
					Content-Length: 88
					Content-Type: text/html
					Connection: Closed

					<html>
					<body>
					<h1>Hello, World!</h1>
					</body>
					</html>
	
	
HTTP-Methods: 

HTTP defines methods to indicate the desired action to be performed on the identified resource. The HTTP/1.0 specification defined the GET, POST and HEAD methods. 
GET:	Requests using GET should only retrieve data and should have no other effect. 
HEAD:	Asks for the response identical to the one that would correspond to a GET 				request, but without the response body.
POST:	Requests that the server accept the entity enclosed in the request as a new 				subordinate of the web resource identified by the URI.
 
##3. HTTP Stateless Protocol (Protocol State Machine)

HTTP functions as a request-response protocol in the client-server computing model and HTTP/1.0 uses a new connection for each request/response. HTTP is connectionless and it is a direct result of HTTP being a stateless protocol. The server and client are aware of each other only during a current request. Afterwards, both of them forget about each other. Due to this nature of the protocol, neither the client nor the browser can retain information between different requests across the web pages.

##4. Overview of Implementation

Code:	 https://github.com/dsandesari/http

Implementation:  
HTTP protocol is implemented over connection oriented TCP protocol that supports IPv4 network. The Client/Server is implemented using socket based programming. This implementation handles only GET and HEAD request and responses. The content types supported are only text/html files. The server supports two types of status or response codes.  
  
HTTP Server:    

To configure the server initially we need to pass port number and root directory of http server where the source files are present. If root directory is not present the server returns error.  
./httpserver 7700 '/Users/ROHITH/http/httpserver/rootdir'

The HTTP Server creates a socket, binds the socket to the address and listens for the connections. The server waits for client connections and accepts until the client connects to the server. Once the client sends the request, the server starts parsing the request and constructs the status message and http response header with optional body. Server checks if the file requested by client available or not. If file is not found the server only sends the status message “HTTP/1.0 404 Not Found” and if file is found then server constructs header with fields such as Date, Server, Content-Type, and Content-Length. The server sends the body of the message if input requested is a GET request or else if it is a HEA request the server omits the body of the message. 


Concurrent Iterative Server:  
  
  The server is iterative as it runs continuously using the while loop until the process is terminated. Once the server accepts a connection and then it creates a child to do the work and once after the child process is done it loop backs and waits for the next connection.    
  
HTTP Client:  
  
  To configure the client initially we need to pass request type, hostname, port number and request file path. The commands line inputs are parsed to check the validity of inputs.
 ./httpclient GET /index.html localhost 7700 HTTP/1.0
The client creates the socket and connects to the socket address of the server. The client then constructs the request message and sends it to the server and waits for the response. The client then receives the http response message and displays it on the standard output.



##5. My Contributions

 Implemented Iterative Concurrent HTTP Server
-	Parse the command line arguments.
-	Create the socket.
-	Bind the socket to the address.
-	Concurrency of the Server (fork)
-	Accept the connections
-	Receive get request from client
-	Construct the HTTP response message  
-	a.	Header (Date, Server, Content-Type (only text/html), Content-Length)  
-	b.	Status message (HTTP/1.0 200 OK or HTTP/1.0 404 Not Found)  
-	c.	Body
-	Handling of GET/HEAD Response 
-	Send response to the client
	
##6. Future Work

As future work we can extend the current implementation to accommodate additional features such as URI, support multiple content types of media to transfer, improve the response header, support multiple response codes, authentication schemes and add feature to support encrypted connections using https. 

##7. References

[1].	HTTP V 0.9, The Original HTTP as defined in 1991, 	http://www.w3.org/Protocols/HTTP/AsImplemented.html  
[2].	Wikipedia HTTP, http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol  
[3]. 	HTTP /1.0, http://tools.ietf.org/html/rfc1945  
[4].	HTTP Made Really Easy, http://www.jmarshall.com/easy/http/  
[5]. 	Socket Programming, http://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming 

