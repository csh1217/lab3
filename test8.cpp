#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main(){
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) return 1;

    string buf = "Hello World";
    
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(10130);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int numBytes = sendto(s, buf.c_str(), buf.length(),
        0, (struct sockaddr *)&sin, sizeof(sin));
    char buf2[65536];
    socklen_t sin_size = sizeof(sin);
    bind(s, (struct sockaddr *) &sin, sizeof(sin));
    
    while(true){
        numBytes = recvfrom(s, buf2, sizeof(buf2), 0,
                            (struct sockaddr *)&sin, &sin_size);
        sendto(s, buf.c_str(), buf.length(),
        0, (struct sockaddr *)&sin, sizeof(sin));
    };
    
    cout << "Sent: " << numBytes << endl;


    memset(&sin, 0, sizeof(sin));
    cout << "Received: " << numBytes << endl;
    cout << "From" << inet_ntoa(sin.sin_addr) << endl;
    
    memset(&sin, 0, sizeof(sin));
    sin_size = sizeof(sin);
    int result = getsockname(s, (struct sockaddr *) &sin, &sin_size);
    if (result == 0){
        cout << "My addr: " << inet_ntoa(sin.sin_addr) << endl;
        cout << "My port: " << ntohs(sin.sin_port) << endl;
    }

    close(s);
    return 0;
}