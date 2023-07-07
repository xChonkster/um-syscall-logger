import json

JSON_WINDOWS_VER = "Windows 10"
JSON_KERNEL_VER = "20H2"

with open("nt-per-system.json", "r") as json_file:
    json_tree = json.loads(json_file.read())

for name in json_tree[JSON_WINDOWS_VER][JSON_KERNEL_VER]:
    print("    \"{name}\",".format(name = name))
