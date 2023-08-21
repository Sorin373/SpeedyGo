import os

os_type = get_operating_system()

if os_type == "Windows":
    os.system("setup/setup.bat")
elif os_type == "Unix":
    os.system("./setup.sh")
else:
    print("Unsupported operating system.")
