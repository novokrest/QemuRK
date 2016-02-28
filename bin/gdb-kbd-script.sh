#set breakpoints to possible keyboard-relative functions

break hid_keyboard_poll
break hid_keyboard_event
break usb_hid_changed
break ps2_put_keycode

#run qemu with usb-passthrough
run -hda test.qcow2 -usb -device usb-host,hostbus=4,hostaddr=20

#run qemu with usb-keyboard emulating
run -hda test.qcow2 -usbdevice keyboard

