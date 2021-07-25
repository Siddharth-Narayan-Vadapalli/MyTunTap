# **MyTunTap**
## **Tun and Tap device implementation for Linux**
### **A. Basics of Tun and Tap devices**
Tun and Tap devices are virtual network devices and not physical devices.<br>
Tun device simulates a network layer device (carries IP packets) while a Tap device simulates a link layer device (carries Ethernet Frames). <br>
Tun and Tap devices are used to deliver packets to and fro between the Linux network stack and the user space program which interfaces with the Tun or Tap device. <br>
They are used in VPN networks and Virtual Machine networks. <br>

### **B. Setting up the Tun device and interfacing with it**
To setup the Tun device, open the terminal in the folder where the .sh files are present.<br>
Then, run the following commands: <br><br>
`$ bash addtun.sh `<br>
`$ bash starttun.sh`<br><br>
Now, we have added the Tun device and have also started it. We can now interact with it by starting the Tun device interface. <br>
Run the following command from a new terminal:<br><br>
`$ ./mytunclient.out`<br><br>
We can see that the Tun device replies to the ping requests that we had started when we ran the starttun.sh file. <br>
To stop and delete the Tun device, run the following commands: <br><br>
`$ bash stoptun.sh`<br>
`$ bash deltun.sh`<br><br>

### **C. Setting up the Tap device and interfacing with it**
To setup the Tap device, open the terminal in the folder where the .sh files are present.<br>
Then, run the following commands: <br><br>
`$ bash addtap.sh `<br>
`$ bash starttap.sh`<br><br>
Now, we have added the Tap device and have also started it. We can now interact with it by starting the Tap device interface. <br>
Run the following command from a new terminal:<br><br>
`$ ./mytapclient.out`<br><br>
We can see that the Tap device replies to the broadcast requests that we had started when we ran the starttap.sh file. <br>
To stop and delete the Tap device, run the following commands: <br><br>
`$ bash stoptap.sh`<br>
`$ bash deltap.sh`<br><br>

### **D. Customizing the device interfaces**
Modify the mytunclient.c and mytapclient.c source codes and compile them to generate new mytunclient.out and mytapclient.out executable files. <br>
You could write custom functions for your desired packet and ethernet frame contents. <br>
In the existing device interface source codes, replies to the ping request and broadcast request for the Tun and Tap devices respectively have been implemented.<br>
Download and use Wireshark: https://www.wireshark.org/ to analyze the packet format and contents to understand how ping and broadcast requests and replies work. <br>

**Note:** I **DO NOT CLAIM** that all of the code hosted in this repository has been written by me entirely. I have referred to multiple websites for portions of the code present in the mytunclient.c and mytapclient.c files as well as for the .sh files. However, I have written the sections of code which deal with replying to ping requests and broadcast requests by analyzing the contents of the request packets using Wireshark. I hope this repository serves as a good starting point for people interested in working with Tun and Tap devices.
