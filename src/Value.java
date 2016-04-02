import java.util.ArrayList;
import java.util.List;
import javax.annotation.Generated;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Value
{
	private List<String> ColumnNames = new ArrayList<String>();
	@SerializedName("ColumnTypes")
	@Expose
	private List<String> ColumnTypes = new ArrayList<String>();
	@SerializedName("Values")
	@Expose
	private List<List<String>> Values = new ArrayList<List<String>>();


	public List<String> getColumnNames() {
		return ColumnNames;
	}

	public void setColumnNames(List<String> ColumnNames) {
		this.ColumnNames = ColumnNames;
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

    @Override
    public String toString()
    {
        return "ClassPojo [Values = "+Values+", ColumnNames = "+ColumnNames+", ColumnTypes = "+ColumnTypes+"]";
    }
}
			
			