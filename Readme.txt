compile the server using the following on linux command line

gcc UDPStructServer.c DieWithError.c -o UDPStructServer

compile the client using:
gcc UDPStructClient.c DieWithError.c -o UDPStructClient


first run the server using the name of the object file and then the port number
eg. port 25220
./UDPStructServer 25220

then run the client in the format,
filename <IP address> <message> <request type> <UserID> <LeaderID>
request is in the form of integers from 1 to 4
UserID and LeaderID have to be 5 digits

./UDPStructClient 127.0.0.1 2 'hello' 1 12345 56789


the github link for the program is https://github.com/ArifSohaib/TwitterAppInC
