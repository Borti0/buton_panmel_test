import json
import socket


def _main():
    localhost = "127.0.0.1"
    listener_port = 55550
    sender_port = 55551

    listener_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    listener_socket.bind((localhost, listener_port))
    sender_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # sender_socket.bind((localhost, sender_port))

    # send json data
    remote_data = {
        "pc_name": "remote",
        "pc_index": 1,
        "command_index": 0,
        "command": "ECHO"
    }

    # main infinite loop
    while True:
        # recv data from udp port
        data, addr = listener_socket.recvfrom(1024)
        data = data.decode('utf8')
        data = json.loads(
            data
        )
        print(f"Recv Data {data}")

        # send back data
        remote_data["command_index"] = data["command_index"]
        json_data = json.dumps(remote_data)  # Convert to JSON string
        sender_socket.sendto(json_data.encode('utf-8'), (localhost, sender_port))


if __name__ == "__main__":
    _main()
