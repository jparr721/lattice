import glob
import os
from typing import Dict, List
import socket
import sys
import logging

import numpy as np
import tensorflow as tf

logging.basicConfig(level=logging.DEBUG)


class SocketServer(object):
    def __init__(self):
        self.SERVER_ADDRESS = "hookenet"
        self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.neural_network = NeuralNetwork()

    def start(self):
        # First, try to remove the existing socket if it"s live
        try:
            os.unlink(self.SERVER_ADDRESS)
        except OSError:
            pass

        self.sock.bind(self.SERVER_ADDRESS)
        self.sock.listen(1)

        try:
            while True:
                logging.info("Waiting for connections")

                conn, c_addr = self.sock.accept()
                try:
                    logging.debug(f"connection from: {c_addr}")

                    # Receive incoming data
                    while True:
                        # Data shouldn't exceed 256 bits probably...
                        data = conn.recv(256).decode("utf-8")

                        if data and len(data) > 3:
                            try:
                                # Get raw input param data
                                k, x = data.split(",")

                                # str -> float
                                k = float(k)
                                x = float(x)

                                nn_input = np.array([[k, x]])

                                prediction = self.neural_network.predict(nn_input)

                                # float -> str -> bytes
                                conn.sendall(str(prediction).encode("utf-8"))
                            except Exception as e:
                                logging.error(f"Something broke: {e} input: {data}")
                        else:
                            logging.debug("Exiting.")
                            break
                finally:
                    conn.close()
        except KeyboardInterrupt:
            print("Ctrl-C Pressed, Exiting.")


def read_csv(self, filename: str) -> Dict[str, List[float]]:
    with open(filename, "r") as f:
        headers = f.readline().rstrip()

        output = {header: [] for header in headers.split(",")[2:]}
        lines = f.readlines()[1:]

        for line in lines:
            _, _, k, x, F = line.rstrip().split(",")
            output["k"].append(float(k))
            output["x"].append(float(x))
            output["F"].append(float(F))

    return output


def find_latest_file(self) -> str:
    return max(glob.glob("../stats/force-stats-*"), key=os.path.getmtime)


class NeuralNetwork(object):
    def __init__(self):
        self.model_file_name = "hookey"

        if os.path.exists(self.model_file_name):
            self.model = tf.keras.models.load_model(self.model_file_name)
        else:
            self.model = self.retrain()

    def predict(self, x: np.array) -> float:
        return self.model.predict(x)[0][0]

    def retrain(self):
        csv = read_csv(find_latest_file())
        csv_transposed_np = np.array(list(zip(*[v for v in csv.values()]))[5000:])
        np.random.shuffle(csv_transposed_np)

        X = csv_transposed_np[:, :2]
        y = csv_transposed_np[:, 2:]

        train_count = int(len(X) * 0.9)
        test_count = len(X) - train_count

        X_train = X[test_count:]
        X_test = X[:test_count]

        y_train = y[test_count:]
        y_test = y[:test_count]

        model = tf.keras.models.Sequential([
            tf.keras.layers.Dense(12, input_dim=2, activation="relu"),
            tf.keras.layers.Dropout(0.5),
            tf.keras.layers.Dense(8, activation="relu"),
            tf.keras.layers.Dense(1, activation="sigmoid")
        ])

        model.compile(optimizer="adam",
                      loss="mean_squared_error",
                      metrics=["accuracy"])

        model.fit(X_train, y_train, epochs=10)
        model.evaluate(X_test, y_test)

        model.save(self.model_file_name)
        return model


if __name__ == "__main__":
    ss = SocketServer()
    ss.start()
    exit(0)
