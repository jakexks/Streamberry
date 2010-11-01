package com.streamberry;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
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
	byte uniqid[] = new byte[6];

	@Override
	public void run() {
		System.out.println("Sender thread starting");

		try {
			Enumeration<NetworkInterface> nics = NetworkInterface
					.getNetworkInterfaces();
			while (nics.hasMoreElements()) {
				NetworkInterface ni = nics.nextElement();
				System.out.print("Found network adaptor " + ni.getName() + " ");
				byte mac[] = ni.getHardwareAddress();
				for (int i = 0; i < mac.length; i++)
					System.out.format("%02X%s", mac[i],
							(i < mac.length - 1) ? "-" : "\n");
				if (mac == null) {
					// No MAC address, probably loopback
					System.err.println("No MAC ???");
					System.exit(1);
				} else {
					uniqid = mac;
					System.out.print("with mac ");
					for (int i = 0; i < mac.length; i++)
						System.out.format("%02X%s", mac[i],
								(i < mac.length - 1) ? "-" : "\n");
					break;
				}
				if (uniqid == null) {
					// TODO: No MAC! random ID generator
				}
			}

		} catch (SocketException e) {
			e.printStackTrace();
			System.exit(1);
		}

		Timer heartbeattimer = new Timer();
		heartbeattimer.scheduleAtFixedRate(new heartbeatsend(), 0, 5000);

	}

	private int send(DatagramSocket s, byte[] data) {
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

	class heartbeatsend extends TimerTask {
		public void run() {
			try {
				DatagramSocket s = new DatagramSocket();
				byte sendtest[] = concat("StreamBerry:HELO:".getBytes("UTF-8"),
						uniqid);

				send(s, sendtest);

			} catch (SocketException e) {
				e.printStackTrace();
				System.exit(1);
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
				System.exit(1);
			}

		}
	}

}
