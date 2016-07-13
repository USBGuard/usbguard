# How to generate the documentation

The manual page source file is written in the markdown format. You can
convert it to roff or HTML using pandoc:

    $ pandoc -s -t man usbguard-daemon.8.md -o usbguard-daemon.8
    $ pandoc -s -t man usbguard-daemon.conf.5.md -o usbguard-daemon.conf.5
    $ pandoc -s -t man usbguard-rules.conf.5.md -o usbguard-rules.conf.5
    $ pandoc -s -t man usbguard.1.md -o usbguard.1
    $ pandoc -s -t man usbguard-applet-qt.1.md -o usbguard-applet-qt.1

To view the generated files you can use the man(1) command:

    $ man ./usbguard-daemon.8

