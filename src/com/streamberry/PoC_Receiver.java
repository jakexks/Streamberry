package com.streamberry;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class PoC_Receiver implements Runnable {

	int port = 35489;	
	// Arbitrarily chosen from unassigned multicast block
	// See:
	// http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xml
	String multicastgroup = "224.0.0.133";
	
	@Override
	public void run() {
		System.out.println("Receiver thread starting");
		listener(port);
	}

	private void listener(int port) {
		byte recvbuf[] = new byte[1024];
		
		try {
			MulticastSocket s = new MulticastSocket(port);

			s.joinGroup(InetAddress.getByName(multicastgroup));
			DatagramPacket p = new DatagramPacket(recvbuf, recvbuf.length);
			for (;;) {
				s.receive(p);
				System.out.println("Recieved data from " + p.getAddress().getCanonicalHostName());
				System.out.println("Contents: "+ new String(p.getData(),"UTF-8"));
			}
		} catch (IOException e) {
			System.err.println("Failed to make multicast socket (port " + port);
			e.printStackTrace();
			System.exit(1);
		}
	}

}
