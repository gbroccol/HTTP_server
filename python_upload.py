#!/usr/bin/python

import os


def multipart_save_file(boundary, path_body):
    with open(path_body, "r") as file_read:
        # tmp = file_read.readline()  # boundary
        # tmp = file_read.readline()  # Content-Disposition: form-data; name="photo"; filename="2.png"
        # tmp = file_read.readline()  # Content-Type: image/png
        # tmp = file_read.readline()
        with open("./content/website1/users/gbroccol/avatar.png", "w") as new_file:  # hardcode
            list_lines = file_read.readlines()
            list_lines = list_lines[4:]
            new_file.writelines(list_lines[:list_lines.index("--" + boundary + "\r\n")])
    # if os.path.exists(path_body):
    #     os.remove(path_body)


for value in os.environ:
    if os.environ[value].find("multipart/form-data; boundary=") >= 0:
        multipart_save_file(os.environ[value][30:], os.environ.get("PATH_TRANSLATED"))


