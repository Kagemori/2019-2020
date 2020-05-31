#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  

using namespace std;
using namespace cgicc;

int main () {
   Cgicc formData;
   
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>Using GET and POST Methods</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";

   form_iterator fi = formData.getElement("first_number");  
   int num1 = stoi(**fi);
   
   if( !fi->isEmpty() && fi != (*formData).end()) {  
      cout << "First number: " << **fi << endl;  
   } else {
      cout << "No text entered for first number" << endl;  
   }
   
   cout << "<br/>\n";
   fi = formData.getElement("second_number");  
   int num2 = stoi(**fi);
   
   if( !fi->isEmpty() &&fi != (*formData).end()) {  
      cout << "Second number: " << **fi << endl;  
   } else {
      cout << "No text entered for second number" << endl;  
   }
   
   cout << "<br/>\n";
   
   int sum = num1 + num2;
   cout << "The sum of " << num1 << " + " << num2 << " = " << sum << endl;
   
   cout << "<br/>\n";
   cout << "</body>\n";
   cout << "</html>\n";
   
   return 0;
}