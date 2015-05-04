# Notes

## USBGuard CLI

### Working with rules
```
usbguard rule append "rule" --parent <seqn> --timeout <sec> --permanent=[true/false]
usbguard rule remove <seqn>
usbguard rule list
```

### Working with devices
```
usbguard device list
usbguard device rule <seqn>
usbguard device hash <seqn>
usbguard device set-policy <seqn> <target> --timeout <sec> --permanent[=true/false]
```

### Working with authorization
```
usbguard auth list
usbguard auth add "pubkey"
usbguard auth remove "pubkey"
```

### Working with command sessions/transactions
```
usbguard session begin
usbguard session end
```
The `session begin` command records the current session ID returned by the currently running USBGuard daemon instance. All the other commands are required to check this ID (iff the session ID file exists) and fail if it differs from the session ID of the running daemon instance.

The `session end` command deletes the session ID file if it exists.

### Known issues

 * Some of the commands take a sequence number as an argument. This number is session specific and if USBGuard is restarted in between two invocations of CLI commands that use or print the number, it might refer to a different device or rule. A solution to this could be the proposed *session* commands described above. Another solution could be to implement transactions in the API, but I don't think such a complex tool is needed in USBGuard.
