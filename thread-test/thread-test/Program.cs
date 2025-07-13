using System;
using System.IO;
using System.Threading;
using System.Text;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;
using System.Reflection.Metadata;
using System.Collections.Concurrent;
using System.Security;
using System.Net.NetworkInformation;

public class Program
{
    public class Commands
    {
        public string? pc_name { get; set; }
        public int? pc_index { get; set; }
        public uint? command_index{ get; set; }
        public string? command { get; set; }
    }

    // create udp recever
    static UdpClient udp_recv = new UdpClient(55551);
    static IPEndPoint remote_ep = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 0);

    // create udp sender
    static Socket udp_sender = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
    static IPAddress local_address = IPAddress.Parse("127.0.0.1");
    static IPEndPoint end_point = new IPEndPoint(local_address, 55550);

    //creating string blocking queue recv
    static BlockingCollection<string> recv_queue = new BlockingCollection<string>();

    //create string blocking queue send
    static BlockingCollection<string> send_queue = new BlockingCollection<string>();

    public static void Main(string[] args)
    {
        CancellationTokenSource src_token = new CancellationTokenSource();
        CancellationToken token = src_token.Token;

        Console.WriteLine("Start threads");
        Thread recv_task = new Thread(() => ReceiveTask(token));
        Thread send_task = new Thread(() => SendTask(token));
        recv_task.Start();
        send_task.Start();

        bool stop_app = false;

        // command to send to the end point
        Commands scmd = new Commands
        {
            pc_name = "Host",
            pc_index = 0,
            command_index = 0,
            command = "PING"
        };
        Console.WriteLine("Executing app:");
        while (!stop_app)
        {
            Thread.Sleep(1000);
            scmd.command_index += 1;
            
            //convert class to json string and adding to queue
            send_queue.Add(
                JsonSerializer.Serialize(scmd)
                );
            Console.WriteLine($"Adding ping command {scmd.command_index} to the send queue");

            // procesing received commands
            // check if data it is in buffer
            if (!recv_queue.TryTake(out string json_recv_string))
            {
                Console.WriteLine("Empty recv queue");
                continue;
            }

            var rcmd = JsonSerializer.Deserialize<Commands>(json_recv_string);
            Console.WriteLine($"{rcmd.pc_name}");
            Console.WriteLine($"{rcmd.pc_index}");
            Console.WriteLine($"{rcmd.command}");
            Console.WriteLine($"{rcmd.command_index}");

            if ( rcmd.command_index == 1000000 && rcmd.pc_name == "remote")
                stop_app = true;
        }

        Console.WriteLine("Close threads");
        src_token.Cancel();
        recv_task.Join();
        send_task.Join();

        Console.WriteLine("Close ports");
        udp_recv.Close();
        udp_sender.Close();

        Console.WriteLine("Close app");
        return;
    }

    public static void SendTask(CancellationToken token)
    {
        uint index = 1;
        while (!token.IsCancellationRequested)
        {
            // check if data it is in buffer
            if(!send_queue.TryTake(out string json_send_string))
            {
                Console.WriteLine("Empty send queue");
                continue;
            }

            byte[] data = Encoding.UTF8.GetBytes(json_send_string);
            udp_sender.SendTo(data, end_point);
            Console.WriteLine($"Sending command {index} via UPD");
            index += 1;
            Thread.Sleep(1000);
        }

        return;
    }

    public static void ReceiveTask(CancellationToken token)
    {
        uint index = 1;
        while (!token.IsCancellationRequested)
        {
            byte[] recv_data = udp_recv.Receive(ref remote_ep);
            Console.WriteLine($"Receiving {index} command via UDP");
            string recive_json_string = Encoding.UTF8.GetString(recv_data);
            recv_queue.Add(recive_json_string);
            Console.WriteLine($"Adding command {index} to the line");
            index += 1;
            Thread.Sleep(1000);
        }
        return;
    }
}