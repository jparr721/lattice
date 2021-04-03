import glob
import os
from typing import Dict, List
import socket
import sys
import logging

import numpy as np
import tensorflow as tf
from tensorflow.keras import backend as K

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


def read_csv(filename: str) -> Dict[str, List[float]]:
    with open(filename, "r") as f:
        headers = f.readline().rstrip()

        output = {header: [] for header in headers.split(",")[1:]}
        lines = f.readlines()[1:]

        for line in lines:
            _, mn, density, k, x, F = line.rstrip().split(",")
            output["mass_number"].append(float(mn))
            output["density"].append(float(density))
            output["k"].append(float(k))
            output["x"].append(float(x))
            output["F"].append(float(F))

    return output


def find_latest_file() -> str:
    return max(glob.glob("../stats/force-stats-*"), key=os.path.getmtime)


class NeuralNetwork(object):
    def __init__(self):
        self.model_file_name = "nn_force"

        if os.path.exists(self.model_file_name):
            self.model = tf.keras.models.load_model(self.model_file_name, custom_objects={
                'polynomial_regression_equality': NeuralNetwork.polynomial_regression_equality})
        else:
            self.model = self.retrain()

    def predict(self, x: np.array) -> float:
        return self.model.predict(x)[0][0]

    @staticmethod
    def polynomial_regression_equality(y_true: float, y_pred: float):
        """
        Regression accuracy metrix
        :param y_true:
        :param y_pred:
        :return:
        """
        accepted_diff = 0.1
        diff = K.abs(y_true - y_pred)
        return K.mean(K.cast(diff < accepted_diff, tf.float32))

    def retrain(self):
        logging.info("Starting training cycle")
        csv = read_csv(find_latest_file())

        logging.info("Data loaded")
        training_data = np.array(list(zip(*[v for v in csv.values()])), dtype=np.float)
        training_data = training_data[np.where(training_data[:, 0] < 4)]
        training_data = training_data[~np.isnan(training_data).any(axis=1)]
        training_data = training_data[~np.isinf(training_data).any(axis=1)]
        training_data = self._standardize(training_data)
        np.random.shuffle(training_data)
        logging.info("Data standardized")

        X = training_data[:, :2]
        y = training_data[:, 2:]

        train_count = int(len(X) * 0.9)
        test_count = len(X) - train_count

        X_train = X[test_count:]
        X_test = X[:test_count]

        y_train = y[test_count:]
        y_test = y[:test_count]

        model = tf.keras.models.Sequential([
            tf.keras.layers.Dense(12, input_dim=2, activation="relu"),
            # Regularize the middle layer
            tf.keras.layers.Dense(8, activation="relu"),
            # kernel_regularizer=tf.keras.regularizers.l1_l2(l1=1e-5, l2=1e-4)),
            tf.keras.layers.Dropout(0.3),
            # Linear activation because the outputs can be unbounded
            tf.keras.layers.Dense(1, activation="linear")
        ])

        model.compile(optimizer="adam",
                      loss="mean_squared_error",
                      metrics=[NeuralNetwork.polynomial_regression_equality])

        model.fit(X_train, y_train, epochs=10)
        model.evaluate(X_test, y_test)

        tf.keras.models.save_model(model, self.model_file_name,
                                   )
        return model

    def _standardize(self, sequence: np.array) -> np.array:
        """
        Standardizes all inputs
        :param sequence:
        :return:
        """
        for i, row in enumerate(sequence):
            sequence[i] = (row - np.mean(row, axis=0)) / np.std(row, axis=0)

        return sequence


if __name__ == "__main__":
    ss = SocketServer()
    ss.start()
    exit(0)
