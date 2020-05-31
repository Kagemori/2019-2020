#!/usr/bin/env python
import cgi, os, shutil, subprocess, smtplib, datetime
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
import cgitb; cgitb.enable()

form = cgi.FieldStorage()
print "Content-type: text/html\r\n"
fileitem = form['zipcodefile']
fileitem2 = form['statefile']

#checks if files are uploadable
if fileitem.filename:
    fn = os.path.basename(fileitem.filename)
    fn = os.path.basename(fileitem.filename.replace("\\","/"))
    open('../CPS3525/upload/' + fn, 'wb').write(fileitem.file.read())
    message = 'The file "' + fn + '" was uploaded successfully'
else:
    message = 'No file was uploaded' 
if fileitem2.filename:
    fn = os.path.basename(fileitem2.filename)
    fn = os.path.basename(fileitem2.filename.replace("\\","/"))
    open('../CPS3525/upload/' + fn, 'wb').write(fileitem2.file.read())
    message2 = 'The file "' + fn + '" was uploaded successfully'
else:
    message2 = 'No file was uploaded'

#print in html format
print """\
<html>
<body> 
<p> %s </p>
<p> %s </p>
</body>
</html>
"""%(message,message2)

#subprocess.call(["javac Project2.java"])

output = subprocess.check_output(["java", "Project2"])
print(output)

currentdt = datetime.datetime.now()
dt = currentdt.strftime("%m/%d/%Y, %H:%M:%S")

sender = "estrekei@yoda.kean.edu"
receiver = "chuang@kean.edu"

msg = MIMEMultipart()
msg['From'] = sender
msg['To'] = receiver
msg['Subject'] = "CPS3525 Project 2 Result"
body = "The Java program was run at " + dt + ", and the result is available at yoda.kean.edu/~estrekei/CPS3525/upload/Project2_results.html"
msg.attach(MIMEText(body,'plain'))
text = msg.as_string()

smtpObj = smtplib.SMTP('localhost')
smtpObj.sendmail(sender, receiver, text)

