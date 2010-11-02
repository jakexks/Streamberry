package com.streamberry;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Timer;
import java.util.TimerTask;

public class PoC_Sender implements Runnable {

	private final int port = 35489;
	// Arbitrarily chosen from unassigned multicast block
	// See:
	// http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xml
	private final String multicastgroup = "224.0.0.133";

	@Override
	public void run() {
		System.out.println("Sender thread starting");

		Timer heartbeattimer = new Timer();
		heartbeattimer.scheduleAtFixedRate(new heartbeatsend(), 0, 5000);
	}

	public byte[] getUniqID() {
		byte[] id = new byte[6];
		try {
			Enumeration<NetworkInterface> nics = NetworkInterface
					.getNetworkInterfaces();
			while (nics.hasMoreElements()) {
				NetworkInterface ni = nics.nextElement();
//				System.out.println("Found network adaptor " + ni.getName()
//						+ " ");
				byte mac[] = ni.getHardwareAddress();
				if (iszero(mac)) {
					// No MAC address, probably loopback
//					System.err.println("No MAC found for " + ni.getName()
//							+ "trying next network interface.");
				} else {

					id = mac;
//					for (int i = 0; i < mac.length; i++) {
//						System.out.format("%02X%s", mac[i],
//								(i < mac.length - 1) ? "-" : "\n");
//					}
					break;
				}

			}
			if (iszero(id)) {
				// TODO: No MAC! random ID generator
			}

		} catch (SocketException e) {
			e.printStackTrace();
			System.exit(1);
		}
		return id;
	}

	public int send(DatagramSocket s, byte[] data) {
		try {
			DatagramPacket p = new DatagramPacket(data, data.length,
					InetAddress.getByName(multicastgroup), port);
			s.send(p);
		} catch (IOException e) {
			System.err.println("Failed to send data");
			e.printStackTrace();
		}
		return 0;
	}

	public byte[] concat(byte[] first, byte[] second) {
		byte[] result = Arrays.copyOf(first, first.length + second.length);
		System.arraycopy(second, 0, result, first.length, second.length);
		return result;
	}

	private boolean iszero(byte[] array) {
		if (array == null)
			return true;
		for (int i = 0; i < array.length; i++) {
			if (array[i] != 0) {
				return false;
			}
		}

		return true;
	}

	class heartbeatsend extends TimerTask {
		public void run() {
			try {
				DatagramSocket s = new DatagramSocket();
				byte hellopacket[] = { (byte) 0xFF, (byte) 0xFF };

				byte sendtest[] = concat(hellopacket, getUniqID());

				send(s, sendtest);

			} catch (SocketException e) {
				e.printStackTrace();
				System.exit(1);
			}

		}
	}

}
