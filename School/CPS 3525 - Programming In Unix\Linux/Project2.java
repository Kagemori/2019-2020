import java.util.*;
import java.io.*;

public class Project2 {
	public static void main(String[] args) throws IOException {
		
		//Reads the state csv and turns it into a csv array
		String pathToCsv = "../CPS3525/upload/project2_state.csv";
		String thisLine; 
		BufferedReader csvReader = new BufferedReader(new FileReader(pathToCsv));
		List<String[]> lines = new ArrayList<String[]>();
		while ((thisLine = csvReader.readLine()) != null) {
			lines.add(thisLine.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1));
		}
		csvReader.close();
		String[][] arrayStates = new String[lines.size()][0];
		lines.toArray(arrayStates);
		
		//Reads the zipcode csv and turns it into a 2d array
		String pathToCsv2 = "../CPS3525/upload/project2_zipcode.csv";
		BufferedReader csvReader2 = new BufferedReader(new FileReader(pathToCsv2));
		List<String[]> lines2 = new ArrayList<String[]>();
		while ((thisLine = csvReader2.readLine()) != null) {
			lines2.add(thisLine.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1));
		}
		csvReader2.close();
		String[][] arrayStateZips = new String[lines2.size()][0];
		lines2.toArray(arrayStateZips);
		
		//Multidimensional array with different types, makes use of type object
		Object[][] tableArr = new Object[arrayStates.length][5];
		tableArr[0][0] = new String("State");
		tableArr[0][1] = new String("Capital");
		tableArr[0][2] = new String("Number_of_zipcodes");
		tableArr[0][3] = new String("Total_Estimated_Population");
		tableArr[0][4] = new String("Average_AvgWages");
		
		//Fills in 2d array for output
		for(int y = 1; y < tableArr.length; y++) {
			tableArr[y][0] = new String(arrayStates[y][1]);
			tableArr[y][1] = new String(arrayStates[y][3]);
			
			String name = (String) tableArr[y][0];
			tableArr[y][2] = countZipcodes(arrayStateZips,name);
			tableArr[y][3] = totalEstPopu(arrayStateZips,name);
			tableArr[y][4] = avgWages(arrayStateZips,name);
		}
		
		//Counts the states where there are zipcodes present
		int arrx = 0;
		int arry = 0;
		for(int i = 1; i < tableArr.length; i++) {
			int temp = (int) tableArr[i][2];
			for(int j = 0; j < tableArr[1].length; j++) {
				if(temp != 0) {
					arry++;
				}
			}
			if(temp != 0) {
				arrx++;
			}
		}
		
		htmlTable(arrx, arry, tableArr, arrayStateZips);
		
	}
	
	//Method that finds and counts the zipcodes of each state using their acronym
	public static int countZipcodes(String [][] arr, String state) {
		int totalZip = 0;
		String stateCsv;
		
		for(int i = 0; i < arr.length; i++) {
			stateCsv = arr[i][4];
			if(state.equalsIgnoreCase(stateCsv)) {
				totalZip = totalZip + 1;
			}
		}
		
		return totalZip;
	}
	
	//Method that sums the estimated population for each state from zipcode csv
	public static int totalEstPopu(String [][] arr, String state) {
		int totalPop = 0;
		String stateCsv;
		
		for(int i = 0; i < arr.length; i++) {
			stateCsv = arr[i][4];
			if(state.equalsIgnoreCase(stateCsv)) {
				int temp = Integer.parseInt(arr[i][17]);
				totalPop = totalPop + temp;
			}
		}
		
		return totalPop;
	}
	
	//Method that sums and averages the average wages of each state
	public static double avgWages(String [][] arr, String state) {
		double averageWages = 0;
		int count = 0;
		String stateCsv;
		
		for(int i = 0; i < arr.length; i++) {
			stateCsv = arr[i][4];
			if(state.equalsIgnoreCase(stateCsv) && arr[i][19].isEmpty() != true) {
				double temp = Double.parseDouble(arr[i][19]);
				averageWages = averageWages + temp;
				count = count + 1;
			}
		}
		
		if(count != 0) {
			averageWages = (averageWages / count );
		}
		
		return averageWages;
	}
	
	//Finds zipcode searched for and returns a string of outputs
	public static String zipLookup(String [][] arr, int zipcode) {
		String output = "";
		
		for(int i = 1; i < arr.length; i++) {
			int temp = Integer.parseInt(arr[i][1]);
			for(int j = 0; j < arr[1].length; j++) {
				if(zipcode == temp) {
					double tempo = Double.parseDouble(arr[i][19]);
					String temps = arr[i][13];
					temps = temps.replace("\"","");
					output = "Zipcode " + zipcode + " at " + temps + " has the maximum population " + arr[i][17] + " with AvgWages $" + round(tempo,1);
				}
			}
		}
		
		return output;
	}
	
	//rounding function
	public static double round(double value, int places) {
	    if (places < 0) throw new IllegalArgumentException();

	    long factor = (long) Math.pow(10, places);
	    value = value * factor;
	    long tmp = Math.round(value);
	    return (double) tmp / factor;
	}
	
	public static void htmlTable(int arrx, int arry, Object [][] arr, String [][] arr2) {
		Object [][] tableArr = new Object[arry][arrx];
		Object [][] sortedTable = new Object[arry][arrx];
		int count = 0;
		
		//Creates separate object array for values that are shown.
		for(int i = 1; i < arr.length; i++) {
			int temp = (int) arr[i][2];
			for(int j = 0; j < arr[1].length; j++) {
				if(temp != 0) {
					tableArr[count][j] = arr[i][j];
				}
			}
			if(temp != 0) {
				count++;
			}
		}
		
		//Sorts the object array by descending avgwage value
		int index = 0; int counter =0; double value = (double)tableArr[0][4];
		while(counter != tableArr[1].length) {
			for(int z = 0; z < tableArr[1].length; z++) {
				if(value < (double)tableArr[z][4]) {
					value = (double) tableArr[z][4];
					index = z;
				}
			}
			tableArr[index][4] = 0.0;
			sortedTable[counter][0] = tableArr[index][0];
			sortedTable[counter][1] = tableArr[index][1];
			sortedTable[counter][2] = tableArr[index][2];
			sortedTable[counter][3] = tableArr[index][3];
			sortedTable[counter][4] = round(value,1);
			counter++;
			value = -1;
		}
		
		//System printout
		System.out.println("Content-type:text/html\r\n\r\n");
		System.out.println("<html>\n");
		System.out.println("<head>\n");
		System.out.println("<style>\r\n table, th, td {\r\n   border: 1px solid black;\r\n }\r\n </style>");
		System.out.println("<title>Project 3 Output</title>\n");
		System.out.println("</head>\n");
		System.out.println("<body>\n");
		System.out.println("<table style=\"width:100%\">");
			System.out.println("<tr>");
				System.out.println("<th>" + arr[0][0] + "</th>");
				System.out.println("<th>" + arr[0][1] + "</th>");
				System.out.println("<th>" + arr[0][2] + "</th>");
				System.out.println("<th>" + arr[0][3] + "</th>");
				System.out.println("<th>" + arr[0][4] + "</th>");
			System.out.println("</tr>");
		for(int i = 0; i < sortedTable[1].length; i++) {
			System.out.println("<tr>");
				System.out.println("<td>" + sortedTable[i][0] + "</td>");
				System.out.println("<td>" + sortedTable[i][1] + "</td>");
				System.out.println("<td align='right'>" + sortedTable[i][2] + "</td>");
				System.out.println("<td align='right'>" + sortedTable[i][3] + "</td>");
				System.out.println("<td align='right'>" + "$" + sortedTable[i][4] + "</td>");
			System.out.println("</tr>");
		}
		System.out.println("</table> <br/>\n");
		System.out.println(zipLookup(arr2, 10156));
		System.out.println("<br/>\n");
		System.out.println("</body>\n");
		System.out.println("</html>\n");
		
		//Writing to file
		try {
			File file = new File("../CPS3525/upload/Project2_results.html");
			FileWriter writer = new FileWriter(file);
			writer.write("<html>\n" + "<head>\n" + "<style>\r\n table, th, td {\r\n   border: 1px solid black;\r\n }\r\n </style>");
			writer.write("<title>Project 3 Output</title>\n" + "</head>\n" + "<body>\n" + "<table style=\"width:100%\">");
			writer.write("<tr>" + "<th>" + arr[0][0] + "</th>" + "<th>" + arr[0][1] + "</th>" + "<th>" + arr[0][2] + "</th>" + "<th>" + arr[0][3] + "</th>" + "<th>" + arr[0][4] + "</th>" + "</tr>");
			for(int i = 0; i < sortedTable[1].length; i++) {
				writer.write("<tr>");
				writer.write("<td>" + sortedTable[i][0] + "</td>");
				writer.write("<td>" + sortedTable[i][1] + "</td>");
				writer.write("<td align='right'>" + sortedTable[i][2] + "</td>");
				writer.write("<td align='right'>" + sortedTable[i][3] + "</td>");
				writer.write("<td align='right'>" + "$" + sortedTable[i][4] + "</td>");
				writer.write("</tr>");
			}
			writer.write("</table> <br/>\n");
			writer.write(zipLookup(arr2, 10156));
			writer.write("<br/>\n");
			writer.write("</body>\n");
			writer.write("</html>\n");
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		
	}
}
