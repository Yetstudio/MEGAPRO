#!/usr/bin/env python3

import serial
import time
import os
import argparse
from tqdm import tqdm


def crc8(page):
    crc = 0x00
    for b in page:
        for _ in range(8):
            sum = (crc ^ b) & 0x01
            crc >>= 1
            if sum:
                crc ^= 0x8C
            b >>= 1
    return crc


def program(port, rom):
    if not os.path.exists(rom):
        print("Rom file dose not exsit.")
        exit()

    firmware = open(rom, "rb")
    size = os.path.getsize(rom)
    tot = 0
    page_size = 64

    # Change the port and Baud rate according to your settings
    ser = serial.Serial(port, 2000000)
    time.sleep(0.5)

    while(ser.inWaiting() <= 0):
        continue

    msg = ser.read(ser.inWaiting())
    if(len(msg) == 1 and int(msg[0]) == 0xcc):
        print("Start sending bytes...")
    else:
        print("Initialize failed.")
        exit()

    pbar = tqdm(total=int(size / page_size))
    while True:
        ser.flushOutput()
        ser.flushInput()
        page = firmware.read(page_size)
        if len(page) == 0:
            break
        ser.write(page)
        local_crc = crc8(page)
        # Best result 0.186
        while(ser.inWaiting() < 2):
            continue
        msg = ser.read(ser.inWaiting())
        if(len(msg) != 2):
            pbar.close()
            print("Error occured when checking page", tot)
            exit()
        if(local_crc != msg[0]):
            pbar.close()
            print("CRC error when sending page", tot)
            exit()
        if(msg[1] != 1):
            pbar.close()
            print("CRC error when writing page", tot)
            exit()
        tot += 1
        pbar.update(1)

    pbar.close()
    print("Done writing %d pages!" % tot)


parser = argparse.ArgumentParser(
    description='Programming script for sending pages data into Arduino Mega 2560.')
parser.add_argument('--port', '-p', help='Serial port of Arduino Mega 2560')
parser.add_argument('--rom', '-r', help='Path to rom file.')
args = parser.parse_args()


if __name__ == '__main__':
    program(port=args.port, rom=args.rom)
