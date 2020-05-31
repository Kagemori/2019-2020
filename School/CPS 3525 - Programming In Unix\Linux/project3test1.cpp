#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;

const string ENV[ 10 ] = {            
   "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         
   "HTTP_HOST", "HTTP_USER_AGENT",            
   "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      
   "REQUEST_METHOD", "REQUEST_URI", "HTTP_REFERER" }; 

int main() {
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>CGI Environment Variables</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<table border = \"0\" cellspacing = \"2\">";
	
	for ( int i = 0; i < 10; i++ ) {
      cout << "<tr><td>" << ENV[ i ] << "</td><td>";
      
      char *value = getenv( ENV[ i ].c_str() );  
      if ( value != 0 ) {
         cout << value;                                 
      } else {
         cout << "Environment variable does not exist.";
      }
      cout << "</td></tr>\n";
   }
	
	cout << "</table>\n";
	cout << "</body>\n";
	cout << "</html>\n";
	return 0;
}