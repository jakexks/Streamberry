package com.streamberry;

public class Streamberry {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Hello, Streamberry user!");
		new Thread(new Receive()).start();
		new Thread(new BeaconSend()).start();
	}
}
