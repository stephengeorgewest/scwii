package networkConsole;

//import java.io.*;
import java.net.*;


public class NetworkConsole
{
	static int BUFFER_LENGTH=1024; 
	//static SocketAddress DEST_PORT = 1025;
	static String MCAST_ADDR = "224.1.1.1";
	public static void main(String args[]) throws Exception
	{
		byte[] b = new byte[BUFFER_LENGTH];
		DatagramPacket dgram = new DatagramPacket(b, b.length);
		MulticastSocket socket = new MulticastSocket(4000); // must bind receive side
		socket.joinGroup(InetAddress.getByName(MCAST_ADDR));

		boolean done=false;
		while(!done) {
		  socket.receive(dgram); // blocks until a datagram is received
		  //System.err.println("Received " + dgram.getLength() +
		  //  " bytes from " + dgram.getAddress());
		  String str = new String(b,0,dgram.getLength());
		  System.out.println(str);
		  dgram.setLength(b.length); // must reset length field!
		  if(str.equalsIgnoreCase("exit"))
		  	done=true;
		}
	}
}
