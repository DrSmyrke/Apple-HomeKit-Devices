import subprocess

Import("env")

def get_firmware_specifier_build_flag():    
	ret = subprocess.run(["git", "rev-list", "--all", "--count"], stdout=subprocess.PIPE, text=True)
	build_version = ret.stdout.strip()
	build_flag = "-D FIRMWARE_REVISION=\\\"" + build_version + "\\\""
	print ("Firmware Revision: " + build_version)
	return (build_flag)

env.Append(
	BUILD_FLAGS=[get_firmware_specifier_build_flag()]
)
