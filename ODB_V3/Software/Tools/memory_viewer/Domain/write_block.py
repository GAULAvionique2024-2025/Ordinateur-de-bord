"""
Write a single 512-byte block to a raw device or file at the specified LBA.
Usage examples:
  # write command string padded to 512 bytes to control LBA (Windows)
  python write_block.py \\\\.\PhysicalDrive2 --lba 131063 --cmd "ODB_CMD:ENABLE" --dry-run

  # write a binary file to LBA 100
  python write_block.py /path/to/image.bin --lba 100 --infile block512.bin

Notes:
- On Windows writing to \\.\PhysicalDriveN requires Administrator privileges.
- Always backup flash before performing writes.
- Use --dry-run to see what would be written without performing the write.
"""

import argparse
import os
import sys
import ctypes
from ctypes import wintypes

BLOCK_SIZE = 512


def hexdump(b, length=64):
    hexstr = ' '.join(f"{x:02X}" for x in b[:length])
    return hexstr


def write_block_raw_win(device, offset, data):
    # Win32 CreateFile + SetFilePointerEx + WriteFile
    GENERIC_WRITE = 0x40000000
    FILE_SHARE_READ = 0x00000001
    FILE_SHARE_WRITE = 0x00000002
    OPEN_EXISTING = 3

    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    CreateFileW = kernel32.CreateFileW
    CreateFileW.restype = wintypes.HANDLE
    CreateFileW.argtypes = [wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
                            wintypes.LPVOID, wintypes.DWORD, wintypes.DWORD, wintypes.HANDLE]

    handle = CreateFileW(device, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                         None, OPEN_EXISTING, 0, None)
    if handle == wintypes.HANDLE(-1).value:
        err = ctypes.get_last_error()
        raise OSError(f"CreateFileW failed, err={err}")

    # SetFilePointerEx
    SetFilePointerEx = kernel32.SetFilePointerEx
    SetFilePointerEx.argtypes = [wintypes.HANDLE, ctypes.c_longlong, ctypes.POINTER(ctypes.c_longlong), wintypes.DWORD]
    if not SetFilePointerEx(handle, ctypes.c_longlong(offset), None, 0):
        err = ctypes.get_last_error()
        kernel32.CloseHandle(handle)
        raise OSError(f"SetFilePointerEx failed, err={err}")

    # WriteFile
    WriteFile = kernel32.WriteFile
    WriteFile.argtypes = [wintypes.HANDLE, wintypes.LPCVOID, wintypes.DWORD, ctypes.POINTER(wintypes.DWORD), wintypes.LPVOID]
    written = wintypes.DWORD(0)
    if not WriteFile(handle, data, len(data), ctypes.byref(written), None):
        err = ctypes.get_last_error()
        kernel32.CloseHandle(handle)
        raise OSError(f"WriteFile failed, err={err}")

    kernel32.CloseHandle(handle)
    return written.value


def write_block_file(device, offset, data):
    # device can be a path to a file-like device (POSIX) or a file
    with open(device, 'r+b') as f:
        f.seek(offset)
        written = f.write(data)
        f.flush()
    return written


def main():
    p = argparse.ArgumentParser()
    p.add_argument('device', help='Device path (e.g. \\\\.\\PhysicalDrive2) or file')
    p.add_argument('--lba', type=int, required=True, help='Logical block address (512-byte blocks)')
    p.add_argument('--infile', help='Path to 512-byte input file (optional). If omitted use --cmd')
    p.add_argument('--cmd', help='ASCII command string to pad into 512-byte block (optional)')
    p.add_argument('--dry-run', action='store_true', help='Do not perform write, only show info')
    args = p.parse_args()

    if not args.infile and not args.cmd:
        print('Either --infile or --cmd must be provided', file=sys.stderr)
        sys.exit(2)

    if args.infile:
        with open(args.infile, 'rb') as f:
            data = f.read()
    else:
        data = args.cmd.encode('ascii')

    if len(data) > BLOCK_SIZE:
        print(f'Input length {len(data)} > {BLOCK_SIZE}', file=sys.stderr)
        sys.exit(2)

    if len(data) < BLOCK_SIZE:
        data = data + b'\x00' * (BLOCK_SIZE - len(data))

    offset = args.lba * BLOCK_SIZE

    print(f'Device: {args.device} LBA: {args.lba} Offset: {offset} bytes')
    print('First 64 bytes hex:', hexdump(data, 64))

    if args.dry_run:
        print('Dry-run: no write performed')
        return

    try:
        if os.name == 'nt' and args.device.startswith('\\\\.\\'):
            written = write_block_raw_win(args.device, offset, data)
        else:
            written = write_block_file(args.device, offset, data)
        print(f'Written bytes: {written}')
    except Exception as e:
        print('Write failed:', e, file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
