# Notes

## usbguard cli

usbguard rule append "rule" --parent <seqn> --timeout <sec> --permanent=[true/false]
usbguard rule remove <seqn>
usbguard rule list

usbguard device list
usbguard device rule <seqn>
usbguard device hash <seqn>
usbguard device set-policy <seqn> <target> --timeout <sec> --permanent[=true/false]

usbguard auth list
usbguard auth add "pubkey"
usbguard auth remove "pubkey"
