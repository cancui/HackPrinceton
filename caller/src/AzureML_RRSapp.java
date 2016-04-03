import java.util.*;
import java.io.*;
import java.nio.file.Path;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.*;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.InputStreamReader;
import java.io.InputStream;

public class AzureML_RRSapp {
	// public static JSONObject inpParms;
    public static String apikey;
    public static String apiurl;
    public static String jsonBody;
    
    static String parentPath = "C:\\Users\\Can\\Eclipse Workspace\\Squadcaller\\src\\";
    static String apiInfoPath = parentPath + "apiInfo.txt";
    static String initPath = parentPath + "init.json";
    static String triggerPath = parentPath + "trigger.txt";
    static String inputPath = parentPath + "input.csv";
    
    /**
     * Read the JSON schema from the file rrsJson.json
     * 
     * @param filename It expects a fully qualified file name that contains input JSON file
     */		
    /*
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
    */
    public static String readJson2(String filename) {
        try {
            File apiFile = new File(filename);
            Scanner sc = new Scanner(apiFile);
            String json;
            json = "";
            while (sc.hasNext()) {
                json += sc.nextLine()+"\n";
            }
            sc.close();
            return json;
        }
        catch (Exception e){
            System.out.println(e.toString());
            return "null";
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
    
    //CSV READER
    public static List<List<String>> readCSV(File file) throws IOException {
    	List<List<String>> inputs = new ArrayList<List<String>>();
    	for (int i = 0; i < 101; i++) {
    		inputs.add(new ArrayList<String>(15));
    		for (int j = 0; j < 15; j++) {
    			inputs.get(i).add("0");
    		}
    	}
    	BufferedReader br = new BufferedReader(new FileReader(file));
    	String s = br.readLine();
    	String in;
    	char cur;
    	String add = "";
    	int row = 0;
    	int col = 0;
    	while (s != null) {
    		//System.out.println(s);
    		for (int i = 0; i < s.length(); i++) {
    			cur = s.charAt(i);
    			if (cur == ',') {
    				//System.out.print(add);
    				//System.out.print(" ");
    				inputs.get(row).set(col,add);
    				col++;
    				add = "";
    				i += 1;
    				cur = s.charAt(i);
    				add += cur;
    				//System.out.print(" ");
    				//inputs.get(col).get(row) += cur;
    			}
    			else {
    				add += cur;
    			}
    		}
    		inputs.get(row).set(col,add);
    		//System.out.print(add);
			add = "";
    		//System.out.println(" ");
    		col = 0;
    		row++;
    		s = br.readLine();
    	}
    	return inputs;
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
    	//String json = "/home/stephen/workspace-jee/HackPrinceton/src/rrsJson.json";
    	String api = apiInfoPath;
    	/*
    	BufferedReader br = new BufferedReader (new InputStreamReader(System.in));
    	json = br.readLine();
    	api = br.readLine();
    	*/
        try {
		
                // read JSON file name
                //String jsonFile = json;
                // read API file name
                String apiFile = api;
            
                // call method to read API URL and key from API file
                readApiInfo(apiFile);
                
                // call method to read JSON input from the JSON file
                //readJson(jsonFile);
                
                // print the response from REST API
                return (rrsHttpPost());
        }
        catch (Exception e) {
            System.out.println(e.toString());
            return ("NULL");
        }
    }
    
    public static void setData(MyoData myo, List<List<String>> data) {
    	Inputs in = myo.getInputs();
    	Input1 in1 = in.getInput1();
    	in1.setValues(data);
    }
    
    static int i = 0;
    
    public static String mapData(String c) {
    	String word = "";
    	if (c.equals("h"))
    		word = "Hello";
    	else if (c.equals("w"))
    		word = "World";
    	else if (c.equals("b"))
    		word = "Bye";
    	else if (c.equals("t"))
    		word = "Thanks";
    	return word;
    }
    
    public static void main(String[] args) throws IOException{
    	// monitor a single file
    	String template = readJson2(initPath);
    	Gson gson1 = new GsonBuilder().create();
    	final MyoData input = gson1.fromJson(template, MyoData.class);
    	
    	TimerTask task = new FileWatcher( new File(triggerPath) ) {
	    protected void onChange( File file ) {
	        // here we code the action on a change
	    	Gson gson = new GsonBuilder().create();
	    	File f = new File(inputPath);
	    	List<List<String>> data = new ArrayList<List<String>>();
	    	try {
	    		data = readCSV(f);
	    	}
	    	catch (IOException e) {}
	    	setData(input, data);
	    	String JSON;
	    	jsonBody = gson.toJson(input, MyoData.class);
	    	Word word;
	    	//System.out.println(i);
	    	//Example obj;
	    	String wordy;
	    	try {
	        	JSON = access();
	        	word = gson.fromJson(JSON, Word.class);
	        	wordy = word.getResults().getOutput1().getValue().getValues().get(0).get(0);	        
	        	System.out.println(wordy);
	    	}
	        catch (IOException e) {
	        	System.out.println("ioe");
	        }
	        
	      }
	    };

	    Timer timer = new Timer();
	    // repeat the check every second
	    timer.schedule( task , new Date(), 100 );
	    System.out.println("run");
	    //CPPStart cpp = new CPPStart();
    	//cpp.start();
    	//File file = new File("/home/stephen/workspace-jee/HackPrinceton/myo_output1.csv");
    	//readCSV(file);
    
    }
    
}