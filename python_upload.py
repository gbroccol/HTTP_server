#!/usr/bin/python

import sys
import os

class bcolors:
    PURPLE = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def multipartSaveFile(boundary, pathBody):
    with open(pathBody, "r") as file_read:
        tmp = file_read.readline()  # boundary
        tmp = file_read.readline()  # Content-Disposition: form-data; name="photo"; filename="2.png"
        tmp = file_read.readline()  # Content-Type: image/png
        tmp = file_read.readline()
        with open("./content/website1/gbroccol/avatar.png", "w") as new_file:  # hardcode
            while True:
                tmp = file_read.readline()
                if tmp.find(boundary) >= 0:
                    break
                new_file.write(tmp)  # boundary
        tmp = file_read.readline()   # Content-Disposition: form-data; name="user"
        tmp = file_read.readline()   # free line
        tmp = file_read.readline()   # {"name":"gbroccol","age":18}
        tmp = file_read.readline()   # boundary


print(bcolors.YELLOW)
for i in os.environ:
    print(i + " = " + os.environ[i])
print(bcolors.ENDC)

# test
fr = os.environ.get("PATH_TRANSLATED")
print("FILE READ - " + fr)



for value in os.environ:
    if value.find("HTTP_Content-Type=multipart/form-data; boundary=") >= 0:
        multipartSaveFile(value[48:], os.environ.get("PATH_TRANSLATED"))








# >>> from urllib.parse import urlparse, parse_qs
# >>> url = 'http://example.com/?foo=bar&one=1'
# >>> parse_qs(urlparse(url).query)
# {'foo': ['bar'], 'one': ['1']}


# print('The command line arguments are:')
# for i in sys.argv:
#     print(i)

# pathBodyFile = "bodyCGI.txt"

# print("I'm in python")

# envPost
# env = ["AUTH_TYPE=Anonymous",
#        "CONTENT_LENGTH=222",
#        "CONTENT_TYPE=",
#        "GATEWAY_INTERFACE=CGI/1.1",
#        "PATH_INFO=",
#        "PATH_TRANSLATED=",
#        "QUERY_STRING=",
#        "REMOTE_ADDR=",
#        "REMOTE_IDENT=",
#        "REMOTE_USER=gbroccol",
#        "REQUEST_METHOD=POST",
#        "REQUEST_URI=",
#        "SCRIPT_NAME=python_upload.py",
#        "SERVER_NAME=",
#        "SERVER_PORT=",
#        "SERVER_PROTOCOL=HTTP/1.1",
#        "SERVER_SOFTWARE=Webserv/1.1",
#        "HTTP_Content-Type=multipart/form-data; boundary=------WebKitFormBoundaryINFrsbM1Q9bGSOuG"]

# print(bcolors.YELLOW)
# for i in env:
#     print(i)
# print(bcolors.ENDC)






# print env
# print(bcolors.PURPLE)
# for i in os.environ:
#     print(i + " = " + os.environ[i])
# print(bcolors.ENDC)

# Content-Disposition: form-data; name="photo"; filename="2.png"
# Content-Type: image/png



