package com.streamberry;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.Calendar;
import java.util.Date;
import java.util.Hashtable;


public class Receive implements Runnable {

	private static final int port = 35489;
	// Arbitrarily chosen from unassigned multicast block
	// See:
	// http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xml
	private static final String multicastgroup = "224.0.0.133";

	private Hashtable<byte[], Date> clienttable = new Hashtable<byte[], Date>();

	@Override
	public void run() {
		System.out.println("Receiver thread starting");
		listener(port);
	}

	/**
	 * Creates a multicast listen socket on the given port. 
	 * @param port Port number
	 */
	private void listener(int port) {
		byte recvbuf[] = new byte[1024];

		try {
			MulticastSocket s = new MulticastSocket(port);

			s.joinGroup(InetAddress.getByName(multicastgroup));
			DatagramPacket p = new DatagramPacket(recvbuf, recvbuf.length);
			for (;;) {
				s.receive(p);
				decode(p);
			}
		} catch (IOException e) {
			System.err.println("Failed to make multicast socket (port " + port);
			e.printStackTrace();
			System.exit(1);
		}
	}

	/**
	 * Determines the type of the incoming datagram packet p, before passing it
	 * to the appropriate class to deal with said packet.
	 * 
	 * @param p
	 *            The incoming DatagramPacket
	 */
	private void decode(DatagramPacket p) {
		System.out.println("Received data from "
				+ p.getAddress().getCanonicalHostName());
		byte data[] = p.getData();
		if (data[0] == (byte) 0xFF && data[1] == (byte) 0xFF) {
			new Thread(new Beacon(data, clienttable)).start();
		}
	}

}

class Beacon extends Thread {
	/**
	 * This class deals with Beacon (keep-alive) packets
	 * @param data The Raw packet data received  
	 * @param ht The hashtable of current other users on the network
	 */
	public Beacon(byte[] data, Hashtable<byte[], Date> ht) {
		byte id[] = { data[2], data[3], data[4], data[5], data[6], data[7] };

		if (ht.containsKey(id)) {
			ht.remove(id);
			Calendar c = Calendar.getInstance();
			ht.put(id, c.getTime());
			System.out.println("updated ht");
		} else {
			Calendar c = Calendar.getInstance();
			ht.put(id, c.getTime());
			System.out.println("added to ht");
		}

		System.out.print("Hello (Keep-alive) packet from ");
		for (int i = 0; i < id.length; i++) {
			System.out
					.format("%02X%s", id[i], (i < id.length - 1) ? "-" : "\n");
		}
	}

}
