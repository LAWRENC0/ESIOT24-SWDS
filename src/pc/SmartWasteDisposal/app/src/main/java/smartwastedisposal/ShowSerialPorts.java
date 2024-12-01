package smartwastedisposal;

import jssc.*;

public class ShowSerialPorts {

	public static void main(String[] args) {

		/* detect serial ports */
		String[] portNames = SerialPortList.getPortNames();
		System.out.println(portNames.length + " ports found");
		for (int i = 0; i < portNames.length; i++) {
			System.out.println(portNames[i]);
		}

	}

}
