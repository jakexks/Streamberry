package com.streamberry;

public class Streamberry {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		new Thread(new PoC_Receiver()).start();
		new Thread(new PoC_Sender()).start();
		System.out.println("Hello, Streamberry user!");

	}
}
