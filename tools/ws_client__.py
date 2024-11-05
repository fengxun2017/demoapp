import websocket
import threading


def on_open(ws_client):
    print("ws_client on_open")

def on_message(ws_client, message):
    print("ws_client on_message:", message)


def on_close(ws_client, close_status_code, close_msg):
    print("ws_client on_close, close state_code={} close_msg={}".format(close_status_code, close_msg))

def on_error(ws_client, error):
    print("ws_client on error:",error)
    ws_client.close()



def connect_and_run(self):
        def _run():
            client = None
            try:
                client = websocket.WebSocketApp("ws://192.168.56.103:8070",
                    on_open = on_open,
                    on_message = on_message,
                    on_close = on_close,
                    on_error = on_error)

            except Exception as e:
                print("connect to {} failed! : {}".format(url, e))
            if client is not None:
                self._sysm_wsclient = client
                client.run_forever()
            print("exit _run() in SysmWsClient")
            t = threading.Thread(target=_run)
            t.start()