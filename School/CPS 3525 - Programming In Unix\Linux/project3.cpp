#include <iostream>
#include <vector>  
#include <string>  
#include <ctime>
#include <stdio.h>  
#include <stdlib.h> 

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  

using namespace std;
using namespace cgicc;

//Object for each pattern found
struct textObject {
	int id;
	int size;
	int centerX;
	int centerY;
};

//This works but only if C++ can support multidimensional arrays without the use of global variables
/*void searchObj(int xpos, int ypos, int width, int height, textObject * obj, int check, char *arr){
	char current;
	char id = obj->id;
	for(int y = ypos; ypos < height; y++){
		for(int x = xpos; xpos < width; x++){
			current = arr[x][y];
			if(current == 'T' && check == 0){
				//change current element to object id
				arr[x][y] = obj->id;
				//increase its size by one
				obj->size = obj->size+1;
				
				//search the right, if it doesnt find any, return here
				searchObj( x+1, y, width, height, obj, 1, arr);
				//search down, if it doesnt find any, return here
				searchObj( x, y+1, width, height, obj, 1, arr);
				//search the left, if it doesnt find any, return here
				searchObj( x-1, y, width, height, obj, 1, arr);
				//search up, if it doesnt find any, return here
				searchObj( x, y+1, width, height, obj, 1, arr);
			}
			
			if(current == 'T' && check == 1){
				//change current element to object id
				arr[x][y] =obj->id;
				//increase its size by one
				obj->size = obj->size+1;
				
				//search the right, if it doesnt find any, return here
				searchObj( x+1, y, width, height, obj, 1, arr);
				//search down, if it doesnt find any, return here
				searchObj( x, y+1, width, height, obj, 1, arr);
				//search the left, if it doesnt find any, return here
				searchObj( x-1, y, width, height, obj, 1, arr);
				//search up, if it doesnt find any, return here
				searchObj( x, y+1, width, height, obj, 1, arr);
			}
			
			if(current != 'T' && check == 1){
				//return to previous recursion
				break;
			}
			
			if(obj->size >= 0){
				break;
			}
		}
		if(obj->size >= 0){
			break;
		}
	}
}*/

int main () {
   Cgicc formData;
   
   //Convert form input to variables
   const_form_iterator fi = formData.getElement("username");
   string name = **fi;
   fi = formData.getElement("gender");
   string gender = **fi;
   fi = formData.getElement("age");
   int age = stoi(**fi);
   fi = formData.getElement("gpa");
   double gpa = stoi(**fi);
   fi = formData.getElement("birthday");
   string birthday = **fi;   
   string interests[4];
   fi = formData.getElement("interest1");
   interests[0] = **fi;
   fi = formData.getElement("interest2");
   interests[1] = **fi;
   fi = formData.getElement("interest3");
   interests[2] = **fi;
   fi = formData.getElement("interest4");
   interests[3] = **fi;
   fi = formData.getElement("term");
   string term = **fi;
   fi = formData.getElement("txtbox");
   string textbox = (*fi).getStrippedValue();
   
   int count = 0;  
   for (int i = 0; textbox[i]; i++) {
       if (textbox[i] != ' ') {
            textbox[count++] = textbox[i];
		}
	}
    textbox[count] = '\0'; 
   
   char a, b, c;
   int x,y,iter,yh,xw;
   iter = 3;
   a = textbox.at(0);
   b = textbox.at(2);
   x = b - 48;
   y = a - 48;
   char arrText[x][y];
   
   for(yh = 0; yh < y; yh++){
		for(xw = 0; xw < x; xw++){
			arrText[xw][yh] = textbox.at(iter);
			iter++;
		}
	} 
   
   //Additional variable information
   //Current date and time
   time_t now = time(0);
   char* dt = ctime(&now);
   
   //User-Information
   string ENV[4] = {"HTTP_HOST", "REMOTE_ADDR", "REMOTE_PORT","HTTP_USER_AGENT"};
   char* userInfo[4];
   for(int i = 0; i < 4; i++){
	   userInfo[i] = getenv(ENV[i].c_str());
   }
   
   //
   
   //Debugging / Checking variables
   ofstream output("../CPS3525/upload/project3out.html");
   output << "<html>\n";
   output << "<head>\n";
   output << "<title>Project 3 Output</title>\n";
   output << "</head>\n";
   output << "<body>\n";
   output << "Current Date: " << dt << endl;
   output << "<br/>\n";
   output << "User Information - Host Name: " << userInfo[0] << " IP Address: " << userInfo[1] << " Port: " << userInfo[2] << " Browser: " << userInfo[3] << endl;
   output << "<br/>\n";
   output << "Name: " << name << endl;
   output << "<br/>\n";
   output << "Gender: " << gender << endl;
   output << "<br/>\n";
   output << "Age: " << age << endl;
   output << "<br/>\n";
   output << "GPA: " << gpa << endl;
   output << "<br/>\n";
   output << "Birthday: " << birthday << endl;
   output << "<br/>\n";
   output << "Interests: " << endl;
   output << "<br/>\n";
   for(int in = 0; in < 4; in++){
	   if(interests[in] != ""){
		   output << " - "<< interests[in] << endl;
		   output << "<br/>\n";
	   }
   }
   output << "Term: " << term << endl;
   output << "<br/>\n";
   output << "<br/>\n";
   for(yh = 0; yh < y; yh++){
		for(xw = 0; xw < x; xw++){
			output << arrText[xw][yh];
		}
		output << "<br/>\n";
	} 
   
   /*textObject obj1;
   searchObj( 0, 0, b, a, &obj1, 0, *arrText);
   output << obj1.id << " " << obj1.size;
   
   output << "<br/>\n";*/
   output << "</body>\n";
   output << "</html>\n";
   
   //Output Link
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>Project 3</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";
   cout << "Form submitted successfully";
   cout << "<br/>\n";
   cout << "<a href=../CPS3525/upload/project3out.html>Link to output here</a>";
   cout << "</body>\n";
   cout << "</html>\n";
   
   return 0;
}