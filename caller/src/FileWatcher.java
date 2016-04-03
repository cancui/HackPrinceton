import java.util.*;
import java.io.*;

public abstract class FileWatcher extends TimerTask {
	private long timeStamp;
	  private File file;

	  public FileWatcher( File file ) {
	    this.file = file;
	    this.timeStamp = file.lastModified();
	  }

	  public final void run() {
	    long timeStamp = file.lastModified();
	    try {
			BufferedReader br = new BufferedReader(new FileReader(file));
			if( this.timeStamp != timeStamp && br.readLine() != null) {
			      this.timeStamp = timeStamp;
			      onChange(file);
			}
		} catch (IOException e) {}
	  }

	  protected abstract void onChange( File file );
}
