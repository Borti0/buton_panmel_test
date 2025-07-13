using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;

class Program
{
    static void Main(string[] args)
    {
        Socket server_socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        IPAddress local_addres = IPAddress.Parse("127.0.0.1");
        byte[] data = new byte[6 + (6 * 16)];
        IPEndPoint endPoint = new IPEndPoint(local_addres, 55555);

        for (int i = 0; i < 6; i++)
            data[i] = 0xFF;
        
        for (int i = 6; i <= (6*16); i += 6)
        {
            data[i] = 0x01;
            data[i+1] = 0x02;
            data[i+2] = 0x03;
            data[i+3] = 0x04;
            data[i+4] = 0x05;
            data[i+5] = 0x06;
        }

        for (int i = 0; i < 65; i++)
        {
            server_socket.SendTo(data, endPoint);
            Console.WriteLine($"message send {data.Length}");
        }
    }
}