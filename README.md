# usb-notify
This application will alert you while running of usb's that have been inserted
into the linux machine.

Behind the scenes it uses the linux device manager udev
to scan the system for new devices of the subsystem usb, and then looks at the
action to see if it's a device being inserted into the machine.

It then uses the libnotify framework to display a message to the current user.

## Building
You will need libudev and libnotify for this application to build.
`make && sudo make install`

## Usage
`$ usb-notify &` in your startup script (~/.xinitrc)

To view the usb-notify logs: `less /var/log/usb-notify/usb-notify.log`
