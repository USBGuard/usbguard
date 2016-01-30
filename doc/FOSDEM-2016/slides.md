title: USBGuard -- Take control over your USB devices
author:
  name: Daniel Kopeček 
  url: https://github.com/dkopecek/usbguard
output: slides.html
controls: true
theme: sudodoki/reveal-cleaver-theme

--

# USBGuard
## take control over your USB devices

_Daniel Kopeček_

--

### From The News (year 2011)

![story headline](badusb-theregister-forslides.png "story headline")

--

### Attack vectors

* USB sticks are usually infected with a malware that relies on the user to run it or on some autorun feature of the system
  * Solution? Disable autorun, educate users
* **Rogue USB devices** that simulate keystrokes, mouse operations or even a network card with a dhcp server already exist
  * Teensy, USBdriveby, ... Cost? ~$20

--

### Attack vectors

* **Reprogramming** of USB devices is possible without special hardware: BadUSB
* **Bugs in USB drivers** `/* TODO: check bounds */`
  * CVE-2013-1680: Heap-based buffer overflow in drivers/usb/class/cdc-wdm.c

--

### Usability vs. Security

* Physically blocking (epoxy) or destroying USB ports is not always desirable

--

### USBGuard: Up and running/protecting in 1 minute (or so...)

```
# dnf install usbguard usbguard-applet-qt
# usbguard generate-policy > /etc/usbguard/rules.conf
# systemctl start usbguard
$ usbguard-applet-qt &

```
![USBGuard Applet Device Dialog](devicedialog.png "USBGuard Applet Device Dialog")

--

### USBGuard: Components

![USBGuard components](components.svg "USBGuard components")

* A firewall-like system for whitelisting and blacklisting USB devices based on their attributes

--

### Advanced and planned features

* Rule conditions
* USB I/O monitoring (in progress)
* Signing devices (?), network-bound encryption
* Better UI for the applet (hopefully with help from a UX designer ;-)
* (your feature or idea)

--

### Limitations

* Cloning devices, faking attributes, ...
* vendor-specific drivers
* USB keyboards usually don't provide a unique serial number, so it's much easier to bypass the policy if you know the vendor and model.

--

### Thank you for listening! Questions?

* Feedback, suggestions and contributions are highly appreciated!
* See known issues, RFEs at https://github.com/dkopecek/usbguard/issues

--

### APPENDIX: Anatomy Of A Rule
```
target [device_id] [device_attributes] [conditions]
```
* target: allow, block, reject
* device id: VENDOR:PRODUCT, VENDOR:*
  ```
  allow 1234:5678
  block ffff:*
  ```
* attributes: name, serial number, port, interfaces, hash
  ```
  allow via-port "1-2"
  reject with-interface any-of { ff:*:* ab:cd:* }
  ```
* conditions: time, device match, random
  ```
  allow with-interface 03:00:01 if !allowed-matches(with-interface 03:00:01)
  reject if random(0.2)
  ```

--

### APPENDIX: Policy Examples

* Allow USB thumb drives only (mass storage)
```
allow with-interface { 08:*:* }
```

--

### APPENDIX: Policy Examples

* Allow a specific Yubikey device to be connected via a specific port
```
allow 1050:0011 name "Yubico Yubikey II" serial "0001234567" via-port "1-2" hash "044b5e168d40ee0245478416caf3d998"
reject via-port "1-2"
```
* Play "Russian roulette" with USB devices
```
allow if random(0.1666)
reject
```
--

### APPENDIX: CLI Usage Examples

```
# List devices
> usbguard list-devices
3: allow 1a6c:1234 serial "0000:00:14.0" name "xHCI Host Controller" via-port "usb1" with-interface 09:00:00 hash "fc3bee3af869e4161f36dbb8836b2d53"
4: allow 22f4:1234 name "Integrated Camera" via-port "1-12" with-interface equals { 03:00:04 01:00:04 } hash "876ae5b57907c32e51cf8e33ea4bb6f6"

# List rules
> usbguard list-rules
1: allow 1a6c:1234 serial "0000:00:14.0" name "xHCI Host Controller" with-interface 09:00:00 hash "fc3bee3af869e4161f36dbb8836b2d53"
2: allow 22f4:1234 name "Integrated Camera" via-port "1-12" with-interface equals { 03:00:04 01:00:04 } hash "876ae5b57907c32e51cf8e33ea4bb6f6"

# Append a new rule to allow USB thumb drives via a specific port
> usbguard append-rule 'allow via-port "2-1" with-interface 08:06:50'
5
```

