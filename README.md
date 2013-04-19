###About

![screenshot](http://i.imgur.com/LacGXJD.png)

This is a graphical representation of packets moving about the IPv4 space.

The screenshot shows the internet as seen from my dorm room. The upper right is 0xFFFFFFFF, the lower left is 0x00000000.

***
###Building

This relies on pcap to build, which unfortunately requires root to run.

This has been built and tested on osx 10.8 and Ubuntu 12.04. It should build on other unix-likes. Because pcap (and I'm not that interested in figuring out winpcap), it will likely not build on Windows.

On osx, make sure you have xcode and such installed, then just type "make" to build it. Run it with "./network".

On ubuntu, have freeglut-dev and libpcap-dev installed, then "make" and ./network

***
###Using

As stated, this requires root to run.

As of now, the available command line options are:  
`-d <device>`, `-i <device>`, and `-3`.  
`-d <device>` specifies \<device\> as your desired network device,  
`-i <device>` works exactly as `-d`.  
`-3` specifies the 3d representation (which is prettier, but harder to interpret).

***
###Colors

Packets are currently colored according to the following color scheme:

TCP:     blue green (RGB 0, 0.5, 1.0)  
UDP:     green      (RGB 0, 1.0, 0)  
ICMP:    blue       (RGB 0, 0, 1.0)  
IP:      green blue (RGB 0, 1.0, 0.5)  
Unknown: red        (RGB 1.0, 0, 0)  

***
###Technical Details

Probably the most interesting bit is my algorithm for mapping an IP address to a point in a 2d plane.

The algorithm is implemented in the graphics.cpp file, function calculatePosition().

I take each octet, and have the 4 most significant bits go towards the Y position, with the 4 least significant bits going to the X. I then bitshift each of these 4 bits to the left by 4 * (3 - (octet position)).

At the end of this, I have an x and a y, which are at most 65535. I then normalize this to opengl's default (-1,-1) to (1,1) window.

***
###Further Work

In the 3d display, I'd like to be able to accurately display the IP address for both planes. This works for the 2d display, but is currently disabled for the 3d display while I figure it out.

Additionally, I'd like to be able to display the planes that IP addresses are coming/going from in the 3d display.
