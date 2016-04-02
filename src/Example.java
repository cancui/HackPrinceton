import javax.annotation.Generated;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class Example
{
    private Results Results;

    public Results getResults ()
    {
        return Results;
    }

    public void setResults (Results Results)
    {
        this.Results = Results;
    }

    @Override
    public String toString()
    {
        return "ClassPojo [Results = "+Results+"]";
    }
}