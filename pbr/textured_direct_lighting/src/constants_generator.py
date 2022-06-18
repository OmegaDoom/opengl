import sys

with open(sys.argv[1], 'rb') as file:
    data = file.read()
    print('const char data[] = R"({})";'.format(data.decode("utf-8")))
