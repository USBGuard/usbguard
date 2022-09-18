function __fish_usbguard_complete_first_subcommand
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 1
        return 0
    end
    return 1
end

set -l subcommands \
    "get-parameter\tGet the value of a runtime parameter" \
    "set-parameter\tSet the value of a runtime parameter" \
    "list-devices\tList all USB devices recognized by the USBGuard daemon" \
    "allow-device\tAuthorize a device to interact with the system" \
    "block-device\tDeauthorize a device" \
    "reject-device\tDeauthorize and remove a device" \
    "list-rules\tList the rule set (policy) used by the USBGuard daemon" \
    "append-rule\tAppend the rule to ther current rule set" \
    "remove-rule\tRemove a rule identified by the rule id from the rule set" \
    "generate-policy\tGenerate a rule set (policy) which authorizes currently connected USB devices" \
    "watch\tWatch the IPC interface events and print them to stdout" \
    "read-descriptor\tRead a USB descriptor from a file and print it in human-readable format" \
    "add-user\tCreate an IPC access control file allowing user/group to use the USBGuard IPC bus" \
    "remove-user\tRemove an IPC access control file associated with the user/group"

complete -c usbguard -x

for part in $subcommands
    set -l cmd (string split -f 1 "\t" "$part")

    complete -c usbguard -x -n "__fish_usbguard_complete_first_subcommand" -a "$cmd" -d (string split -f 2 "\t" "$part")
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -s "h" -l "help" -d "Show help"
end

for cmd in get-parameter set-parameter
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd; and not __fish_seen_subcommand_from InsertedDevicePolicy; and not __fish_seen_subcommand_from ImplicitPolicyTarget" -a "InsertedDevicePolicy ImplicitPolicyTarget"
end
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter" -s "v" -l "verbose" -d "Print the previous and new attribute value"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from InsertedDevicePolicy" -a "block"        -d "Deauthorize every present device"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from InsertedDevicePolicy" -a "reject"       -d "Remove every present device"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from InsertedDevicePolicy" -a "apply-policy" -d "Evaluate the ruleset for every present device"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from ImplicitPolicyTarget" -a "allow"        -d "Authorize the device"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from ImplicitPolicyTarget" -a "block"        -d "Block the device"
complete -c usbguard -x -n "__fish_seen_subcommand_from set-parameter; and __fish_seen_subcommand_from ImplicitPolicyTarget" -a "reject"       -d "Remove the device"

complete -c usbguard -x -n "__fish_seen_subcommand_from list-devices" -s "a" -l "allowed" -d "List allowed devices"
complete -c usbguard -x -n "__fish_seen_subcommand_from list-devices" -s "b" -l "blocked" -d "List blocked devices"
complete -c usbguard -x -n "__fish_seen_subcommand_from list-devices" -s "t" -l "tree"    -d "List devices in a tree format"

for cmd in allow-device block-device reject-device
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -s "p" -l "permanent" -d "Make the decision permanent"
end

complete -c usbguard -x -n "__fish_seen_subcommand_from list-rules" -s "d" -l "show-devices" -d "Show all devices which are affected by the specific rule"
complete -c usbguard -x -n "__fish_seen_subcommand_from list-rules" -s "l" -l "label"        -d "Only show rules having a specific label"

complete -c usbguard -x -n "__fish_seen_subcommand_from append-rule" -s "a" -l "after"     -d "Append the new rule after a rule with the specified rule id"
complete -c usbguard -x -n "__fish_seen_subcommand_from append-rule" -s "t" -l "temporary" -d "Make the desicion temporary"

complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "p" -l "with-ports"   -d "Generate port specific rules for all devices"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "P" -l "no-ports-sn"  -d "Don't generate port specific rules for devices without an iSerial value"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "d" -l "devpath"      -d "Only generate a rule for devices at the specified sub path of /sys"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "t" -l "target"       -d "Generate an explicit \"catch all\" rule with the specified target"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "X" -l "no-hashes"    -d "Don't generate a hash attribute for each device"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "H" -l "hash-only"    -d "Generate a hash-only policy"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "L" -l "ldif"         -d "Generate a ldif policy for LDAP"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "b" -l "usbguardbase" -d "Generate a ldif policy for LDAP with specified base"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "o" -l "objectclass"  -d "Generate a ldif policy for LDAP with specified objectClass"
complete -c usbguard -x -n "__fish_seen_subcommand_from generate-policy" -s "n" -l "name-prefix"  -d "Generate a ldif policy for LDAP with specified name prefix"

complete -c usbguard    -x -n "__fish_seen_subcommand_from watch" -s "w" -l "wait" -d "Wait for IPC connection to become available"
complete -c usbguard    -x -n "__fish_seen_subcommand_from watch" -s "o" -l "once" -d "Wait only when starting, if needed"
complete -c usbguard -F -r -n "__fish_seen_subcommand_from watch" -s "e" -l "exec" -d "Run an executable file located at specified path for every event"

for cmd in add-user remove-user
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -a "(__fish_complete_users)"
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -a "(__fish_complete_user_ids)"
    complete -c usbguard -n "__fish_seen_subcommand_from $cmd" -s "u" -l "user" -d "Specified name representing a username or UID (default)"

    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -s "g" -l "group" -a "(__fish_complete_groups)"
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -s "g" -l "group" -a "(__fish_complete_group_ids)"
    complete -c usbguard -x -n "__fish_seen_subcommand_from $cmd" -s "g" -l "group" -d "Specified name representing a group name or GID"
end
complete -c usbguard -n "__fish_seen_subcommand_from add-user" -s "p" -l "policy"     -d "Policy related previleges"
complete -c usbguard -n "__fish_seen_subcommand_from add-user" -s "d" -l "devices"    -d "Device related privileges"
complete -c usbguard -n "__fish_seen_subcommand_from add-user" -s "e" -l "exceptions" -d "Exceptions related privileges"
complete -c usbguard -n "__fish_seen_subcommand_from add-user" -s "P" -l "parameters" -d "Run-time parameter related privileges"
