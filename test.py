import re

regex = re.compile(r'[^a-zA-Z0-9 _\-\.]')

def san(name):
    return regex.sub('', name)

print(san("../../hello"))