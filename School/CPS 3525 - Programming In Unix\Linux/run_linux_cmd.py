#!/usr/bin/env python
import cgi, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()
print "Content-type: text/html\n\n" # must have for HTML header
fileitem = form['filename'] # Get filename here.
if fileitem.filename: # Test if the file was uploaded
    # strip leading path from file name to avoid directory traversal attacks
    fn = os.path.basename(fileitem.filename)
    # fn = os.path.basename(fileitem.filename.replace("\\", "/" )) # for Unix/Linux
    open('/tmp/' + fn, 'wb').write(fileitem.file.read())
    message = 'The file "' + fn + '" was uploaded successfully'
else:
    message = 'No file was uploaded'
print """\
<html>
<body> <p>%s</p> </body>
</html>
""" % (message,)