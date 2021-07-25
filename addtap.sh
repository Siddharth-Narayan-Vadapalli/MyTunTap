sudo ip tuntap add mode tap dev tap0
ip addr add 10.10.0.0/24 dev tap0
ip link set address 00:00:00:00:00:01 dev tap0
ip link set dev tap0 up
