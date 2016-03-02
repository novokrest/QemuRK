#set breakpoints to possible keyboard-relative functions

#break hid_keyboard_poll
#break hid_keyboard_event
#break usb_hid_changed
#break ps2_put_keycode
break usb_host_handle_fd
break usb_host_handle_data
break usb_host_req_complete_data

#run qemu with usb-passthrough
run -enable-kvm -hda test.qcow2 -usb -device usb-host,hostbus=?,hostaddr=?

#run qemu with usb-keyboard emulating
run -hda test.qcow2 -usbdevice keyboard

#o
