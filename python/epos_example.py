import matplotlib.pyplot as plt
import time
import numpy as np
from MotorController import *

def main():
    motorController = MotorController()

    motorController.connect("EPOS2", "MAXON SERIAL V2", "USB", "USB0", 1000000)

    # nodeId = 1

    # motorController.activatePositionMode(nodeId)
    # motorController.setPos(nodeId, 10)

    # print('Pos:', pos)


if __name__ == "__main__":
    main()