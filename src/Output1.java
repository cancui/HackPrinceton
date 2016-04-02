import javax.annotation.Generated;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Output1
{
    private Value value;

    private String type;

    public Value getValue ()
    {
        return value;
    }

    public void setValue (Value value)
    {
        this.value = value;
    }

    public String getType ()
    {
        return type;
    }

    public void setType (String type)
    {
        this.type = type;
    }

    @Override
    public String toString()
    {
        return "ClassPojo [value = "+value+", type = "+type+"]";
    }
}
			
			