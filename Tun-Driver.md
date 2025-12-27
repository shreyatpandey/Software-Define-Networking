# Universal TUN/TAP Driver

The **Universal TUN/TAP driver** is a feature of the Linux kernel that allows userspace programs to emulate a network device. It is essentially a bridge between the operating system's networking stack and a user application.

## 1. The Concept: "Software Network Card"
Normally, a network interface (like `eth0` or `wlan0`) is backed by physical hardware. When the kernel sends a packet to `eth0`, the hardware driver sends it out over a wire or radio.

With the **tun driver**, the "hardware" is actually your C++ program.
*   **OS to App**: When the kernel sends a packet to the `tun0` interface, the driver takes that packet and makes it available for your program to `read()` from the file descriptor.
*   **App to OS**: When your program `write()`s data to the file descriptor, the driver takes it and injects it into the kernel's networking stack as if it had just arrived from a physical wire.

## 2. TUN vs. TAP
The driver supports two modes, determined by the flags you pass to `ioctl` (e.g., `IFF_TUN` or `IFF_TAP`).

*   **TUN (Network Layer / Layer 3)**:
    *   Simulates a point-to-point network layer device.
    *   It operates with **IP packets** (or IPv6).
    *   You don't see Ethernet headers (MAC addresses).
    *   *Use case:* VPNs (like OpenVPN in `tun` mode), simple IP tunnels.

*   **TAP (Link Layer / Layer 2)**:
    *   Simulates an Ethernet device.
    *   It operates with **Ethernet frames**.
    *   You see the full Ethernet header (Source MAC, Dest MAC, EtherType).
    *   *Use case:* Virtual Machines (bridging a VM to a physical network), complex VPNs that need to carry non-IP traffic.

## 3. The Role of `/dev/net/tun`
This file is a **multiplexing device**.
*   There is only one file node `/dev/net/tun` on the system.
*   Every program that wants a tunnel opens this *same* file.
*   The `ioctl(TUNSETIFF)` call is what separates them. It tells the kernel, "Create a *new, unique* instance of a network interface (like `tun0`, `tun1`) for *this specific* open file descriptor."

## 4. Visual Flow
```text
+---------------------+
|   User Application  |  <-- Your C++ code
|      (read/write)   |
+----------^----------+
           | File Descriptor (fd)
+----------v----------+
|   /dev/net/tun      |  <-- The Driver
+----------^----------+
           |
+----------v----------+
|  Network Interface  |  <-- e.g., 'tun0' (Created by ioctl)
|      (Kernel)       |
+----------^----------+
           |
      OS Network Stack
      (Routing, Firewall, etc.)
```
