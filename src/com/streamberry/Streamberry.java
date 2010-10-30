package com.streamberry;
public class Streamberry {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		new Thread(new PoC_Threadtest()).start();
		new Thread(new PoC_Threadtest()).start();
		System.out.println("Hello, Streamberry user!");

	}
}
