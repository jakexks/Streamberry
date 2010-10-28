package com.streamberry;

public class PoC_Threadtest implements Runnable {
	// public static void main(String[] args) {
	// new Thread(new PoC_Discovery()).start();
	//		
	// }

	@Override
	public void run() {
		try {
			printwithdelay("Hello", 1);
			printwithdelay("World", 2);
			printwithdelay("Stuff", 3);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void printwithdelay(String thing, int id) throws InterruptedException {
		Thread.sleep(1000);
		System.out.println(thing + "\nI am thread" + id +"\n-------------");
		
	}

}
