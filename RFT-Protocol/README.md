# RFT Protocol: Daisy Bell and Matt Hapner

## Description
The goal of this project it to implement a reliable file transfer (RFT) protocol as an additional service on top of UDP.


## Build
In this repository contains you will find four source files written in c: `src/client.c`, `src/server.c`, `src/helpers.c`, and `src/state.c`. In addition, you will find accompanying header files and a CMake file at `src/CMakeLists.txt`.
Note: the server must be started first and then the client soon after. 

**client.c - client program**
- Four command line arguments:
1. SERVER_IP: ip address of the RFT server
2. SERVER_PORT: network port number where server is listening
3. REMOTE_PATH: filesystem path on the server for the file to be downloaded
4. LOCAL_PATH: filesystem path on the client machine where to store the downloaded file

**Server.c - server-side program**
- One command line argument:
1. LISTENING_PORT: network port number where server is listening



## Packet Sequence Diagrams

![Image description](https://repo.cse.taylor.edu/group-work/dbell-mhapner/blob/master/20200424_152049.jpg)
![Image description](https://repo.cse.taylor.edu/group-work/dbell-mhapner/blob/master/20200424_152021.jpg)
![Image description](https://repo.cse.taylor.edu/group-work/dbell-mhapner/blob/master/20200424_152114.jpg)

## Design Specifications
The way that we have decided to implement RFT is very similar to how TCP implements reliable data transfer, only more minimized. The guarantees that we provide is that our design overcomes lost and out of sequence packets and the contents of those packets are unaltered/uncorrupted.

There are several key components that make this work...
Each packet sent from the server contains both a **sequence** and an **acknowledgement** number, which relate to the number of total bytes sent so far. The client will respond back with the largest **continuous** acknowledgement number it has received so far, and it is the job of the server to re-send packets if the client gives a negative acknowledgement back or if it sends the same positive acknowledgement back even after the server has already sent the next set of packets. The main goal here is to keep the client and server on similar states. We never want the client too far behind or the server too far ahead. In addition, when messages are sent either way, we are always waiting for a response from the other side before moving on to the next step. If no response is received within 2 seconds, we re-send the message and wait again. This process continues for up to 8 total times.

This specification also uses checksums to guarantee the integrity of the data being sent over the line. This is a principle component of any RFT protocol. Every packet sent over the line is formatted like this: **size of data** | **sequence number** | **acknowledgement number** | **checksum** | **padding** | **data**


## Extra notes for later - ignore for milestone 1

Sequence of events:
1. Start server
2. Start client
    - Verify the local_path that the Client specifies
        * If legitimate and won't override existing files, move on to next step
        * Exit with error message
2. Client sends a UDP request to server for a file
    * Performs retry attempts if needed
3. Server responds
    * If file exists, send success message with number of total bytes, number of packets, and size of each packet
        A. Keep trying up to 8 times?
    * If file does not exist or some other error, send error message
4. Client receives initial acknowledgement from server
    * If server sent a message with file information, respond back with positive acknowledgement
        A. Keep trying up to 8 times?
5. Server receives acknowledgement from client - starts sending file data
    * 
    * 
    * Layout of packet:
    * header:
        size | seq number -> start at 0 (running count of bytes) 
        ack number -> seq number + size of packet | checksum | padding | data

    Packet 0: 20 | 00 | 20 | efoihjefos | | Hello world
    Packet 1: 35 | 20 | 35 | 09if0e43jf | | Hello world, you are beautiful
    
If client process is terminated - make sure to delete partially retrieved file

Considerations:
- size of each packet
- 



## Testing Procedure
For our testing procedure we did four basic tests. 

The first test simply had a router working as normal and we sent over the specified file and tried receiving the same file. This worked as expected and diff showed no difference in the files.

For the second test, we added a delay in the router through the "tc qdisc add dev ens37 root netem delay 25ms" command, this delays the packets by 25ms. After we did this, we ran diff on both files, the original and the one we sent over the network, and they were both identical. 

For the third test, we added a packet drop to the router thought the command "tc qdisc add dev ens37 root netem loss 25%". This would drop 25% of the packets, and running diff on both final packets showed that they were the same. 

For the final test, we added packet reordering to the router through the "tc qdisc add dev ens37 root netem reorder 25% delay 1ms" command, which reorders the packets 25% of the time, and diff returned saying the files were the same. 
