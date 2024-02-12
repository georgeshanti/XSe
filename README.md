# XSe::XServer

Read the docs at <https://georgeshanti.github.io/XSe>

XSe is a lightweight HTTP/1.1 webserver library for C++. It is designed to be just a webserver and nothing more. Primary object is to build a  high performance application server that can be run on lower spec machines. XSe uses the standard library for data strcutures and pthreads for threading. The functions to setup APIs and routes are inspired by ExpressJS and Flask.

Things XSe::XServer will do:
1. Open a network socket on a specified port
2. Listen for incoming connections
3. Parse the HTTP request received on a connection into a request object
4. Determine if a function has been defined for the the determined route
4. Provide a way define a function to examine the parsed request and form a response object which will be parsed back into the HTTP response form and written back to the connection
5. Provide a way to add additional middleware to the application server

Things XSe::XServer will not do:
1. It will not parse the body of the HTTP request into an appropriate format depending on Content-Type Header
2. It will not parse objects into certain formats such as JSON, XML for the request body
3. It will not do any logging

Any additional functionality such as request body parsing and response body forming can be written as middleware or separately invoked functions. For example a middleware to parse the response body into JSON/Form data can be added to the application server.

## Install
```
$ cmake .
$ make xse
$ make install
```

The build currently runs for linux so for those who don't have a linux machine there is a Dockerfile and a script(env.sh) to run the docker file with the repo mounted into the container at /xse 
```
$ docker build . -t xse
$ ./env.sh
root@xxxxxxxxxxxx:/# cd xse
root@xxxxxxxxxxxx:/# cmake .
root@xxxxxxxxxxxx:/# make xse
root@xxxxxxxxxxxx:/# make install
```

## Usage
```
//server.cpp
#include<xse/XServer.hpp>

int main(int argc, char *argv[]){

    XSe::XServer xse(8080);

    xse.route("/", std::vector<XSe::HttpMethod>{XSe::PUT}, [](XSe::Request req, XSe::Response res){
        res.setHeader("Content-Type", "application/json");
        res.setBody(std::string("{\"status\": \"1\", \"message\": \"This is the response of PUT method!\"}"));
        res.send();
    });

    xse.route("/", std::vector<XSe::HttpMethod>{XSe::GET, XSe::POST}, [](XSe::Request req, XSe::Response res){
        res.setHeader("Content-Type", "application/json");
        res.setBody(std::string("{\"status\": \"1\", \"message\": \"This is the response of GET and POST methods!\"}"));
        res.send();
    });

    xse.start();
}
```
To build and run
```
$ g++ server.cpp -lpthread -lxse -o a.out
$ ./a.out
```
### Let's break it down
1. Include the header file 'xse/XServer.h'
```
#include<xse/XServer.hpp>
```
2. Create an instance of the server on port 8080
```
XSe::XServer xse(8080);
```
    XServer(int port)

    Desc
        - Creates an instance of XServer listening on a specified port

    Arguments
        - port: the port on which the server should be running
    
    Return
        - XServer instance
3. Add a function for route '/' for the PUT Method
```
xse.route("/", std::vector<XSe::HttpMethod>{XSe::PUT}, [](XSe::Request req, XSe::Response res){
    // Function definition for the route
});
```
    void XServer::route(std::string route, std::vector<XSe::HttpMethod> methods, XSe::Handler handler)

    Desc
        - Attaches a function to a route and set of allowed methods

    Arguments
        - route: a string specifying the route to use the function for
        - methods: a vector of XSe::HttpMethods to determine which
                   methods must be allowed for this function
        - handler: a function of the format
                    void [](XSe::Request, XSe::Respone)
                 Here an anonymous function that accepts two parameters,
                 a request object and a response object is passed.
                 When this route is called with any of the specified
                 methods, this function is called and the response
                 and request objects are passed to it.
    
    Return
        - void
4. Set the Content-Type header for the response
```
res.setHeader("Content-Type", "application/json")
```
    void Response::setHeader(std::string key, std::string value)

    Desc
        - Sets a header key-value pair for the response

    Arguments
        - key: string specifying the header name
        - value: string specifying the header value

    Return
        - void
5. Set the response body
```
res.setBody("{\"status\": \"1\", \"message\": \"This is the response of GET and POST methods!\"}")
```
    void Response::setBody(std::string body)

    Desc
        - Sets the body for the response and also sets
          the Content-Length header to the length of the body

    Arguments
        - body: string to be set as response body. Here a JSON string is set

    Return
        - void

    
6. Send the response back to the client
```
res.send()
```
    Response.send()

    Desc
        - Prepare the response, send it back to the connection and close the connection
    
    Arguments
        - None
    
    Return
        - None


## Extra things you can do
### Examine the request
```
req.getHeader("Content-Type") 		// Get value of Content-Type header, any header name
                                       can be passed to get the corresponding header value
req.getBody()						// Get the body of the request as a string
req.getPath()						// Get the path of the request
req.getMethod()						// Get the method of the request -> XSe::HttpMethod object
```
### Customize the response
```
res.setBody(std::string)			// Set the response body
res.setStatus(XSe::HttpStatus)		// Set the response status
res.setHeader(std::string key, std::string value)		// Set a header for the response
```
### Write middleware for the server
```
// Initialize the server
XServer xse(8080);

// Get the default function with getHandler() and store it into the variable oldHandler
XSe::Handler oldHandler = xse.getHandler();

// Define a new function newHandler
// This middleware will add the header 'Server' with value 'XSe::XServer'
// to every request before it is processed by a route function
XSe::Handler newHandler = [oldHandler](XSe::Request req, XSe::Response res){
    res.setHeader("Server", "XSe::XServer");
    oldHandler(req, res);
};

// Set the default function to newHandler
xse.setHandler(newHandler);

// Start the server
xse.start();
```
### Hardcore God Mode ON
___WARNING: THIS IS EXTREMELY RISKY FUNCTIONALITY AND WITH GREAT POWER COMES GREAT RESPONSITBILITY. PLEASE USE THIS FEATURE VERY CAREFULLY OTHERWISE YOU MIGHT END UP WITH DANGLING POINTERS AND MEMORY LEAKS. YOU HAVE BEEN WARNED.___

The request class has a field called 'extras' which is of type std::map<std::map, void*>. It is essentially a dictionary that maps strings to void pointers. So you can dynamically allocate a variable of any type, cast it to void* and store it in extra against a mapping. Be very careful with this fucntionality because if memory space is not deleted then memory leaks can happen and then you can say bye bye to your system memory.

In this example we write a middleware that adds a dynamically created string object to the extras field of the request object for every incoming request and a route in which this field is returned, cast back into a string pointer and used to make the JSON response.

    Points to remember
        - Extra fields must be dynamically created with the new keyword
        - The vairable must be cast to type (void*) before adding to extras
        - On retrieving the field from extras the value must be explicity cast back to the actual type
```
XServer xse(8080);

XSe::Handler oldHandler = xse.getHandler(); 

XSe::Handler newHandler = [oldHandler](XSe::Request req, XSe::Response res){
    std::string *session_id = new std::string("12345");		    // Dynamically create a string object
    req.setExtra("session_id", (void*)(session_id));		    // Cast to (void*) and add to extras
    oldHandler(req, res);                                       // Call default handler
    delete session_id;											// Delete the variable and free up the memory ince finished
};

xse.setHandler(newHandler);

xse.route("/session", XSe::Methods{XSe::GET}, [](XSe::Request req, XSe::Response res){
    res.setHeader("Content-Type", "application/json");
    std::string *session_id = new std::string("");				// Create a default string
    auto session_id_ptr = req.extras.find("session_id");		// Search of the field in extras
    if(session_id_ptr != req.extras.end()){
        delete session_id;										// If found delete the previously allocated string
        session_id = (std::string*)(session_id_ptr->second);	// Retrieve the value and cast to string type
    }
    // Add the value to the response body
    res.setBody(std::string("{\"status\": \"1\", \"session_id\": \""+(*session_id)+"\"}"));
    res.send();
});

xse.start();
```
