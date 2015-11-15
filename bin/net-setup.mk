QEMU_ROOT = ..

IMAGE = debian-8.2.0-i386-netinst.iso
DISK  = test.qcow2

ETH0IPADDR = 192.168.30.184
MASK       = 255.255.255.0
GATEWAY    = 192.168.30.2
BROADCAST  = 192.168.0.255

TAP_NAME = tap0

all: ifup run

build:
	$(QEMU_ROOT)/configure \
	--target-list=i386-softmmu \
	--enable-debug \
	--disable-pie \
	--enable-debug-info \
	--enable-debug-tcg 
	make -j4

disk:
	rm -f $(DISK)
	./qemu-img create -f qcow2 $(DISK) 3G

setup-os:
	i386-softmmu/qemu-system-i386 -m 1024 -enable-kvm \
	-drive if=virtio,file=$(DISK),cache=none \
	-cdrom $(IMAGE)

run:
	i386-softmmu/qemu-system-i386 -m 1024 -enable-kvm \
	-hda $(DISK) 

run-inet:
	i386-softmmu/qemu-system-i386 -m 1024 -enable-kvm \
	-hda $(DISK) \
	-net nic -net tap,ifname=$(TAP_NAME),script=no,downscript=no

tundev:
	sudo mkdir /dev/net
	sudo mknod /dev/net/tun c 10 200
	sudo modprobe tun

ifup:
	@echo "First take eth0 down, then bring it up with IP address 0.0.0.0"
	sudo ifdown eth0
	sudo ifconfig eth0 0.0.0.0 promisc up
	
	@echo "Bring up the tap device (name specified as first argument, by QEMU)"
	sudo openvpn --mktun --dev $(TAP_NAME) --user `id -un`
	sudo ifconfig $(TAP_NAME) 0.0.0.0 promisc up
	
	@echo "create the bridge between eth0 and the tap device"
	sudo brctl addbr br0
	sudo brctl addif br0 eth0
	sudo brctl addif br0 $(TAP_NAME)
	
	@echo "only a single bridge so loops are not possible, turn off spanning tree protocol"
	sudo brctl stp br0 off 
	
	@echo "Bring up the bridge with ETH0IPADDR and add the default route"
	sudo ifconfig br0 $(ETH0IPADDR) netmask $(MASK) broadcast $(BROADCAST)
	sudo route add default gw $(GATEWAY)

ifdown:
	@echo "Bring down eth0 and br0"
	sudo ifdown eth0
	sudo ifdown br0
	sudo ifconfig br0 down 
	
	@echo "Delete the bridge"
	sudo brctl delbr br0 
	
	@echo "bring up eth0 in 'normal' mode"
	sudo ifconfig eth0 -promisc
	sudo ifup eth0 
	
	@echo "delete the tap device"
	sudo openvpn --rmtun --dev $(TAP_NAME)

#e1000_receive


