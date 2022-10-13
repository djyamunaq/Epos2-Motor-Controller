from MotorController import *

def main():
    motorController = MotorController()

    motorController.connect("EPOS2", "MAXON SERIAL V2", "USB", "USB0", 1000000)

    while True:
        pos = input('Next Target Position: ')
        pos = int(pos)

        motorController.startMovement(1, pos, 0, 0)

if __name__ == "__main__":
    main()