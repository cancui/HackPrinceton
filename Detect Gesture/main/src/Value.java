import java.util.ArrayList;
import java.util.List;
import javax.annotation.Generated;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Value {
	private List<String> ColumnNames = new ArrayList<String>();
	private List<String> ColumnTypes = new ArrayList<String>();
	private List<List<String>> Values = new ArrayList<List<String>>();
	public List<String> getColumnNames() {
		return ColumnNames;
	}
	public List<String> getColumnTypes() {
		return ColumnTypes;
	}
	public void setColumnTypes(List<String> ColumnTypes) {
		this.ColumnTypes = ColumnTypes;
	}
	public List<List<String>> getValues() {
		return Values;
	}
	public void setValues(List<List<String>> Values) {
		this.Values = Values;
	}
	
}
