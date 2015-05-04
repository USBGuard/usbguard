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
### Known issues

 * Some of the commands take a sequence number as an argument. This number is session specific and if USBGuard is restarted in between two invocations of CLI commands that use or print the number, it might refer to a different device or rule.
