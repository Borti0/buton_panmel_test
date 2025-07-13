using System.Text;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;


public class Program
{
    public class Command
    {
        public string? PC_Name { get; set; }
        public int? PC_Index { get; set; }
        public string? PC_Command { get; set; }
    }

    public static void Main(string[] args)
    {
        /* 
        // Sending Json data to the client
        Command cmd = new Command
        {
            PC_Name = "host",
            PC_Index = 0,
            PC_Command = "connect"
        };


        string json_str = JsonSerializer.Serialize(cmd);

        Socket server_socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        IPAddress local_addres = IPAddress.Parse("127.0.0.1");
        byte[] data = Encoding.UTF8.GetBytes(json_str);
        IPEndPoint endPoint = new IPEndPoint(local_addres, 55555);

        for (int i = 0; i < 10; i++)
        {
            server_socket.SendTo(data, endPoint);
            Console.WriteLine($"message send {data.Length}");
            cmd.PC_Index += 1;
            json_str = JsonSerializer.Serialize(cmd);
            data = Encoding.UTF8.GetBytes(json_str);
        }
        */

        // receiving data from server

        UdpClient receiver = new UdpClient(55555);
        IPEndPoint remoteEP = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 0);

        byte[] receivedData = receiver.Receive(ref remoteEP);
        string receivedJson = Encoding.UTF8.GetString(receivedData);

        // Deserialize
        var cmd = JsonSerializer.Deserialize<Command>(receivedJson);
        Console.WriteLine($"{cmd.PC_Name }");
        Console.WriteLine($"{cmd.PC_Index }");
        Console.WriteLine($"{cmd.PC_Command }");
    }
}
