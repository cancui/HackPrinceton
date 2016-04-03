import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;

public class CPPStart extends Thread{
	public void run() {
		Runtime run = Runtime.getRuntime();
		try {
			Process pp = run.exec("/home/stephen/workspace-cpp/HackPrinceton/Debug/HackPrinceton");
			BufferedReader in = new BufferedReader (new InputStreamReader(pp.getErrorStream()));
			Scanner scan = new Scanner(pp.getInputStream());
			int exitVal = pp.waitFor();
			System.out.println("Process exitValue: " + exitVal);
		}
		catch (Exception e) {
			System.out.println("error");
		}
	}
}
