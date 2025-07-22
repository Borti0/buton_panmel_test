//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iostream>
//
//#pragma comment(lib, "ws2_32.lib")
//
//int main() {
//    WSADATA wsaData;
//    SOCKET recvSocket;
//    sockaddr_in localAddr;
//
//    const char* groupIP = "239.255.0.1"; // multicast group address
//    const int port = 5000;
//
//    char buffer[1024];
//
//    // Init Winsock
//    WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//    // Create socket
//    recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//
//    // Allow address reuse
//    BOOL reuse = TRUE;
//    setsockopt(recvSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
//
//    // Bind to local address and port
//    localAddr.sin_family = AF_INET;
//    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    localAddr.sin_port = htons(port);
//
//    bind(recvSocket, (SOCKADDR*)&localAddr, sizeof(localAddr));
//
//    // Join multicast group
//    ip_mreq mreq;
//    mreq.imr_multiaddr.s_addr = inet_addr(groupIP);
//    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
//
//    setsockopt(recvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
//
//    std::cout << "Joined multicast group " << groupIP << " on port " << port << "\n";
//
//    while (true) {
//        sockaddr_in senderAddr;
//        int senderSize = sizeof(senderAddr);
//        int bytes = recvfrom(recvSocket, buffer, sizeof(buffer) - 1, 0,
//            (SOCKADDR*)&senderAddr, &senderSize);
//
//        if (bytes > 0) {
//            buffer[bytes] = '\0';
//            std::cout << "Received: " << buffer << "\n";
//        }
//    }
//
//    closesocket(recvSocket);
//    WSACleanup();
//    return 0;
//}

#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>  // for std::setw and std::setfill

int main() {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    // Simulate some work
    std::this_thread::sleep_for(microseconds(2500));  // 2.5 milliseconds
    while (true)
    {
        auto end = high_resolution_clock::now();
        auto total_micro = duration_cast<microseconds>(end - start).count();

        long seconds = total_micro / 1'000'000;
        long milliseconds = (total_micro % 1'000'000) / 1'000;
        long microseconds = total_micro % 1'000;

        std::cout << "Elapsed time: "
            << seconds << "."
            << std::setw(3) << std::setfill('0') << milliseconds << "."
            << std::setw(3) << std::setfill('0') << microseconds
            << " (sec.millisec.microsec)\n";
    }
    return 0;
}