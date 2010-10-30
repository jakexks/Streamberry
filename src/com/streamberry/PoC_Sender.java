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

public class PoC_Sender implements Runnable {

	int port = 35489;
	String multicastgroup = "224.0.0.133";

	@Override
	public void run() {
		System.out.println("Sender thread starting");

		byte uniqid[] = new byte[6];
		try {
			Enumeration<NetworkInterface> nics = NetworkInterface
					.getNetworkInterfaces();
			while (nics.hasMoreElements()) {
				NetworkInterface ni = nics.nextElement();
				System.out.print("Found network adaptor " + ni.getName() + " ");
				byte mac[] = ni.getHardwareAddress();
				if (mac == null) {
					// No MAC address, probably loopback
				} else {
					uniqid = mac;
					System.out.print("with mac ");
					for (int i = 0; i < mac.length; i++)
						System.out.format("%02X%s", mac[i],
								(i < mac.length - 1) ? "-" : "\n");
					break;
				}
				if (uniqid == null){
					// TODO: No MAC! random ID generator
				}
			}

		} catch (SocketException e) {
			e.printStackTrace();
			System.exit(1);
		}

		try {
			DatagramSocket s = new DatagramSocket();
			byte sendtest[] = concat("StreamBerry:HELO:".getBytes("UTF-8"), uniqid);
			
			send(s, sendtest);

		} catch (SocketException e) {
			e.printStackTrace();
			System.exit(1);
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			System.exit(1);
		}

	}

	private int send(DatagramSocket s, byte[] data) {
		try {
			DatagramPacket p = new DatagramPacket(data, data.length,InetAddress.getByName(multicastgroup),port);
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

}
