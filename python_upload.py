#!/usr/bin/python

import os


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


# |          Mode          |  r   |  r+  |  w   |  w+  |  a   |  a+  |
# | :--------------------: | :--: | :--: | :--: | :--: | :--: | :--: |
# |          Read          |  +   |  +   |      |  +   |      |  +   |
# |         Write          |      |  +   |  +   |  +   |  +   |  +   |
# |         Create         |      |      |  +   |  +   |  +   |  +   |
# |         Cover          |      |      |  +   |  +   |      |      |
# | Point in the beginning |  +   |  +   |  +   |  +   |      |      |
# |    Point in the end    |      |      |      |      |  +   |  +   |


def multipart_save_file(boundary, path_body):
    with open(path_body, "r") as file_read:
        # tmp = file_read.readline()  # boundary
        # tmp = file_read.readline()  # Content-Disposition: form-data; name="photo"; filename="2.png"
        # tmp = file_read.readline()  # Content-Type: image/png
        # tmp = file_read.readline()
        with open("./content/website1/users/" + os.environ.get("REMOTE_USER") + "/avatar.png", "w") as new_file:  # hardcode
            list_lines = file_read.readlines()
            list_lines = list_lines[4:]
            new_file.writelines(list_lines[:list_lines.index("--" + boundary + "\r\n")])
    # if os.path.exists(path_body):
    #     os.remove(path_body)


def check_user_exists(login):
    with open("./content/website1/authentication/authentication.txt", "r") as log_pas_write:
       str = log_pas_write.readlines()
       if "" in

        # check if such user exists
        # exist_log_pass_dict = dict(x.split("_") for x in log_pas_write.readlines().split("1"))
        # print(exist_log_pass_dict)

def login_password_good(login, password):


def sign_up(dict_data):
    with open("./content/website1/authentication/authentication.txt", "r+") as log_pas_write:
        log = dict_data.get("loginSignUp")
        password = dict_data.get("passwordSignUp")
        # check if such user exists
        # exist_log_pass_dict = dict(x.split("_") for x in log_pas_write.readlines().split("1"))
        # print(exist_log_pass_dict)
        log_pas_write.write(log + "_" + password + "true\r\n")
        directory = "./content/website1/users/" + log
        if not os.path.exists(directory):
            os.makedirs(directory)


def login(dict_data):
    with open("./content/website1/authentication/authentication.txt", "r+") as log_pas_write:
        log = dict_data.get("login")
        password = dict_data.get("password")

        log_pas_write.write(log + "_" + password + "true\r\n")
        directory = "./content/website1/users/" + log
        if not os.path.exists(directory):
            os.makedirs(directory)


def authentication(input_data):  # loginSignUp=gg&passwordSignUp=gg
    dict_data = dict(x.split("=") for x in input_data.split("&"))
    if "loginSignUp" in dict_data:
        sign_up(dict_data)
    elif "login" in dict_data:
        login(dict_data)


for value in os.environ:
    if os.environ[value].find("multipart/form-data; boundary=") >= 0:
        multipart_save_file(os.environ[value][30:], os.environ.get("PATH_TRANSLATED"))


if "QUERY_STRING" in os.environ:
    authentication(os.environ.get("QUERY_STRING"))
    # with open(os.environ.get("PATH_TRANSLATED"), "r") as file_read:
    #     print(file_read.read())
    #     print("\r\n")

