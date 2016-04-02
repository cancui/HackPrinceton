import java.util.*;
import java.io.*;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.*;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class AzureML_RRSapp {
	// public static JSONObject inpParms;
    public static String apikey;
    public static String apiurl;
    public static String jsonBody;
    
    /**
     * Read the JSON schema from the file rrsJson.json
     * 
     * @param filename It expects a fully qualified file name that contains input JSON file
     */		
    public static void readJson(String filename) {
        try {
            File apiFile = new File(filename);
            Scanner sc = new Scanner(apiFile);
            jsonBody = "";
            while (sc.hasNext()) {
                jsonBody += sc.nextLine()+"\n";
            }
            sc.close();
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
    }
    
    /**
     * Read the API key and API URL of Azure ML request response REST API
     * 
     * @param filename fully qualified file name that contains API key and API URL
     */	
    public static void readApiInfo(String filename) {
        
        try {
            File apiFile = new File(filename);
            Scanner sc = new Scanner(apiFile);
            
            apiurl = sc.nextLine();
            apikey = sc.nextLine();
            sc.close();
        }
        catch (Exception e){
            System.out.println(e.toString());
        }
        
    }
    
    /**
     * Call REST API for retrieving prediction from Azure ML 
     * @return response from the REST API
     */	
    public static String rrsHttpPost() {
        
        HttpPost post;
        HttpClient client;
        StringEntity entity;
        
        try {
            // create HttpPost and HttpClient object
            post = new HttpPost(apiurl);
            client = HttpClientBuilder.create().build();
            
            // setup output message by copying JSON body into 
            // apache StringEntity object along with content type
            entity = new StringEntity(jsonBody, HTTP.UTF_8);
            entity.setContentEncoding(HTTP.UTF_8);
            entity.setContentType("text/json");

            // add HTTP headers
            post.setHeader("Accept", "text/json");
            post.setHeader("Accept-Charset", "UTF-8");
        
            // set Authorization header based on the API key
            post.setHeader("Authorization", ("Bearer "+apikey));
            post.setEntity(entity);

            // Call REST API and retrieve response content
            HttpResponse authResponse = client.execute(post);
            
            return EntityUtils.toString(authResponse.getEntity());
            
        }
        catch (Exception e) {
            
            return e.toString();
        }
    
    }
    
    /**
     * @param args the command line arguments specifying JSON and API info file names
     */
    public static String access() throws IOException{
        // check for mandatory arguments. This program expects 2 arguments 
        // first argument is full path with file name of JSON file and 
        // second argument is full path with file name of API file that contains API URL and API Key of request response REST API
        /*
    	if (args.length < 2) {
			System.out.println("Incorrect usage. Please use the following calling pattern");
			System.out.println("java AzureML_RRSApp <jsonFilename> <apiInfoFilename>");
        }
        */
    	String json = "/home/stephen/workspace-jee/HackPrinceton/src/rrsJson.json";
    	String api = "/home/stephen/workspace-jee/HackPrinceton/src/apiInfo.txt";
    	/*
    	BufferedReader br = new BufferedReader (new InputStreamReader(System.in));
    	json = br.readLine();
    	api = br.readLine();
    	*/
        try {
		
                // read JSON file name
                String jsonFile = json;
                // read API file name
                String apiFile = api;
            
                // call method to read API URL and key from API file
                readApiInfo(apiFile);
                
                // call method to read JSON input from the JSON file
                readJson(jsonFile);
                
                // print the response from REST API
                return (rrsHttpPost());
        }
        catch (Exception e) {
            System.out.println(e.toString());
            return ("NULL");
        }
    }
    
    public static void main(String[] args) throws IOException{
    	// monitor a single file
    	TimerTask task = new FileWatcher( new File("/home/stephen/workspace-jee/HackPrinceton/src/trigger.txt") ) {
	    protected void onChange( File file ) {
	        // here we code the action on a change
	    	Gson gson = new GsonBuilder().create();
	    	String JSON;
	    	Example obj;
	    	try {
	        	JSON = access();
	        	System.out.println(JSON);
	        	obj = gson.fromJson(JSON, Example.class);
	        	System.out.println(obj);
	        }
	        catch (IOException e) {
	        	System.out.println("ioe");
	        }
	      }
	    };

	    Timer timer = new Timer();
	    // repeat the check every second
	    timer.schedule( task , new Date(), 1000 );
	    System.out.println("run");
    }
}