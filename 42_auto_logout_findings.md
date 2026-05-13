# 42 Network Auto-Logout Mechanisms & Bypasses

This document outlines the findings regarding the automated logout mechanisms implemented on 42 network workstations (specifically Codam) and how they can be successfully bypassed without requiring `sudo` privileges.

## The Logout System

The workstations employ a custom systemd service named `codam-web-greeter-idler.service`, which runs a bash script (`/usr/share/codam/codam-web-greeter-idler.sh` from the [codam-web-greeter](https://github.com/codam-coding-college/codam-web-greeter) repository) every minute via a systemd timer to enforce the 42-minute auto-logout rule.

### The Two Timers Mechanism
The idler script monitors two separate metrics to determine if a user should be logged out:

1. **`TIME_SINCE_LOCK`**: When the screen is locked using `ft_lock` (which redirects to `dm-tool switch-to-greeter`), a temporary file is created at `/tmp/codam_web_greeter_lock_timestamp_$USER`. This file contains the exact UNIX timestamp of when the lock occurred.
2. **`IDLE_TIME`**: The script uses `/usr/bin/xprintidle` to read the X11 session's idle time directly.

- **The logic:** The idler script checks both timers. If **either** `IDLE_TIME` **or** `TIME_SINCE_LOCK` exceeds 42 minutes (2520000 ms), it forces a logout using `/usr/bin/loginctl terminate-user "$USERNAME"` and restarts `lightdm`. This kills all running applications and returns the PC to the login screen, which looks identical to a system crash.
- **Why xdotool fails:** When the screen is locked via `ft_lock`, the system switches the Virtual Terminal (VT) to the greeter. When your user's VT is inactive in the background, Xorg ignores XTEST simulated inputs (like `xdotool` mouse moves). Therefore, `xprintidle` never resets.
- **The bypass:** To prevent the forced logout, we must reset both timers:
  1. The `TIME_SINCE_LOCK` can be reset by simply overwriting `/tmp/codam_web_greeter_lock_timestamp_$USER` with the current UNIX timestamp.
  2. The `IDLE_TIME` can be reset cleanly without injecting fake XTEST keyboard/mouse events (which cause session corruption) by directly invoking the X11 API function `XResetScreenSaver`. This explicitly resets the X server's internal idle timer and works flawlessly even when the VT is inactive in the background.

## The Safe Bypass Implementation

The bypass can be implemented as a safe bash script that resets both the timestamp file and the X11 idle timer using a minimal Python one-liner via `ctypes`. This avoids the severe side effects of rogue keyboard/mouse injections while successfully tricking both checks in the `codam-web-greeter-idler.sh` script.

A minimal safe implementation that accepts a duration in minutes:
```bash
#!/bin/bash

# Prompt for duration if not provided as argument
if [ -z "$1" ]; then
    read -p "Enter duration to keep the session alive (in minutes): " DURATION
else
    DURATION=$1
fi

if ! [[ "$DURATION" =~ ^[0-9]+$ ]]; then
    echo "Error: Duration must be a positive integer."
    exit 1
fi

TIMESTAMP_FILE="/tmp/codam_web_greeter_lock_timestamp_${USER}"

# Pre-create the timestamp file
date +%s > "$TIMESTAMP_FILE"
chmod 666 "$TIMESTAMP_FILE" 2>/dev/null || true

# 1. Update the timestamp and reset X11 idle timer every 60 seconds
(
    END_TIME=$(( $(date +%s) + DURATION * 60 ))
    while [ $(date +%s) -lt $END_TIME ]; do
        # Reset TIME_SINCE_LOCK
        date +%s > "$TIMESTAMP_FILE"
        
        # Reset IDLE_TIME (xprintidle) directly via X11 API
        python3 -c "import ctypes; x11 = ctypes.cdll.LoadLibrary('libX11.so.6'); d = x11.XOpenDisplay(None); x11.XResetScreenSaver(d) if d else None; x11.XCloseDisplay(d) if d else None" 2>/dev/null || true
        
        sleep 60
    done
) &
PID=$!

# 2. Trap cleanup to kill the background loop when the script exits
trap "kill $PID 2>/dev/null; exit 0" INT TERM

# 3. Lock the screen
/usr/local/bin/ft_lock

# 4. Wait for user to return or duration to expire
if read -t $((DURATION * 60)) -s -r -p "Press Enter to stop the keep-alive process early..."; then
    echo -e "\nKeep-alive stopped early."
else
    echo -e "\nKeep-alive duration ended."
fi
kill $PID 2>/dev/null
```

## File Locations of Interest
- **Idler Service:** `/etc/systemd/system/codam-web-greeter-idler.service`
- **Idler Timer:** `/etc/systemd/system/codam-web-greeter-idler.timer`
- **Idler Script (Runtime):** `/usr/share/codam/codam-web-greeter-idler.sh` (Source: [codam-web-greeter GitHub](https://github.com/codam-coding-college/codam-web-greeter/blob/main/systemd/system/codam-web-greeter-idler.sh))
- **Lock Timestamp File:** `/tmp/codam_web_greeter_lock_timestamp_$USER`
- **Official Lock Utility:** `/usr/local/bin/ft_lock`
