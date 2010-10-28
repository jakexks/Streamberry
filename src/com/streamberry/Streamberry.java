package com.streamberry;

public class Streamberry {

	/**
	 * @param args
	 */
	// This is a comment added by Jake to test git
	public static void main(String[] args) {
		System.out.println("Hello, world!");
		new Thread(new PoC_Threadtest()).start();
		new Thread(new PoC_Threadtest()).start();
	}

}
