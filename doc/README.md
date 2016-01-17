# How to generate the documentation

The manual page source file is written in the markdown format. You can
convert it to roff or HTML using ronnjs:

    $ ronn-nodejs --build --roff --manual 8 usbguard-daemon.ronn
    $ ronn-nodejs --build --roff --manual 5 usbguard-daemon.conf.ronn
    $ ronn-nodejs --build --roff --manual 5 usbguard-rules.conf.ronn
    $ ronn-nodejs --build --roff --manual 1 usbguard.ronn

To view the generated files you can use the nroff(1) and less(1) commands:

    $ nroff -u0 -Tlp -man <file>.roff | less

