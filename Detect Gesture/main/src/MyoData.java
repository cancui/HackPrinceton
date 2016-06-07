import javax.annotation.Generated;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class MyoData {
	private Inputs Inputs;
	private GlobalParameters GlobalParameters;
	public Inputs getInputs() {
		return Inputs;
	}
	public void setInputs(Inputs Inputs) {
		this.Inputs = Inputs;
	}
	public GlobalParameters getGlobalParameters() {
		return GlobalParameters;
	}
	public void setGlobalParameters(GlobalParameters GlobalParameters) {
		this.GlobalParameters = GlobalParameters;
	}
}
