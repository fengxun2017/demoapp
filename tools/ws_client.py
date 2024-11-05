import websocket
import threading
import time


class SysmWsClient:
    def __init__(self, server_url):
        # ws://127.0.0.1:13002/ws
        self._server_url = server_url
        self._sysmclient_thread = None
        self._wsclient = None

    def _on_open(self, ws):
        print("ws_client on_open")
        self._wsclient = ws

    def close(self):
        if self._wsclient is not None:
            self._wsclient.close()
            self._wsclient = None

    def _on_message(self, ws_client, message):
        print("ws_client on_message:", message)


    def _on_close(self, ws_client, close_status_code, close_msg):
        print("ws_client on_close, close state_code={} close_msg={}".format(close_status_code, close_msg))
        self._sysmclient_thread = None
        print("release sysm_wsclient")
        
    def _on_error(self, ws_client, error):
        self._sysmclient_thread = None
        print("ws_client on error:",error)
        self.close()

    def connect_and_run(self):
        def _run():
            client = None
            try:
                client = websocket.WebSocketApp(self._server_url,
                                    on_open=self._on_open,
                                    on_message=self._on_message,
                                    on_close=self._on_close,
                                    on_error = self._on_error)
            except Exception as e:
                client = None
                print("connect to {} failed! : {}".format(self._server_url, e))
            if client is not None:
                client.run_forever()
            print("exit _run() in SysmWsClient")

        self._sysmclient_thread = threading.Thread(target=_run)
        self._sysmclient_thread.start()


    def send_msg(self, msg):
        if self._wsclient is not None:
            self._wsclient.send(msg)
        else:
            print("self._sysm_wsclient is None")

    def send(self, msg, opcode):
        if self._wsclient is not None:
            self._wsclient.send(msg, opcode)
        else:
            print("self._sysm_wsclient is None")


if __name__ == '__main__':
    ws_client1 = SysmWsClient("ws://192.168.56.103:8070")
    ws_client1.connect_and_run()
    msg = ["aaabbbbccccc", "ddddeeeefffff"]
    index = 0

    ws_client2 = SysmWsClient("ws://192.168.56.103:8070")
    ws_client2.connect_and_run()
    binary_data = [b'\x01\x02\x03\x04', b'\x05\x06\x07\x08', b'\x31\x32\x33\x34']
    index = 0
    while(1):
        time.sleep(3)

        ws_client2.send(binary_data[0], opcode=websocket.ABNF.OPCODE_BINARY)
        ws_client2.send(binary_data[1], opcode=websocket.ABNF.OPCODE_BINARY)
        ws_client2.send(binary_data[2], opcode=websocket.ABNF.OPCODE_BINARY)
        ws_client1.send_msg(msg[0])
        ws_client1.send_msg(msg[1])