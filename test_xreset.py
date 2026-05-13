#!/usr/bin/env python3
"""Test whether XResetScreenSaver resets xprintidle's idle timer."""
import ctypes
import subprocess
import time

def get_idle():
    try:
        result = subprocess.run(['xprintidle'], capture_output=True, text=True, timeout=5)
        if result.returncode == 0:
            return int(result.stdout.strip())
    except Exception as e:
        print(f"xprintidle error: {e}")
    return None

def reset_screensaver():
    try:
        x11 = ctypes.cdll.LoadLibrary('libX11.so.6')
        d = x11.XOpenDisplay(None)
        if d:
            x11.XResetScreenSaver(d)
            x11.XCloseDisplay(d)
            return True
    except Exception as e:
        print(f"XResetScreenSaver error: {e}")
    return False

print("Testing XResetScreenSaver effect on xprintidle...")
print("=" * 50)

idle1 = get_idle()
print(f"Idle time before XResetScreenSaver: {idle1} ms ({idle1/1000:.1f}s)" if idle1 else "Could not read idle time")

print("Waiting 3 seconds...")
time.sleep(3)

idle2 = get_idle()
print(f"Idle time after 3s wait:            {idle2} ms ({idle2/1000:.1f}s)" if idle2 else "Could not read idle time")

print("Calling XResetScreenSaver...")
reset_screensaver()

time.sleep(0.5)

idle3 = get_idle()
print(f"Idle time after XResetScreenSaver:  {idle3} ms ({idle3/1000:.1f}s)" if idle3 else "Could not read idle time")

print("=" * 50)
if idle1 and idle3:
    if idle3 < 1000:
        print("RESULT: XResetScreenSaver DOES reset xprintidle! (idle ~0 after reset)")
    else:
        print(f"RESULT: XResetScreenSaver does NOT reset xprintidle! (idle still ~{idle3/1000:.1f}s after reset)")
else:
    print("RESULT: Could not determine (xprintidle not available?)")
