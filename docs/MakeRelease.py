import os
import shutil

build_root = "C:\\Users\\noutrey\\Documents\\Perso\\PlatformIO\\Projects\\iSpindHub\\.pio\\build\\"
releases_root = "C:\\Users\\noutrey\\Documents\\Perso\\Brassage\\iSpindHub\\Releases"
release_index = "0.0.7"

list_firmwares = os.scandir(build_root)
for el in list_firmwares:
    if not(el.name == 'project.checksum' ):
        rel_name = el.name
        # Build All FileSystems
        base_cmd = 'C:\\Users\\noutrey\\.platformio\\penv\\Scripts\\platformio.exe run --target buildfs --environment ' + rel_name
        os.system(base_cmd)
        #Get FW File, Copy it to release dir and rename it        
        fw_file_source = build_root + rel_name  + "\\firmware.bin"
        
        fw_file_target = releases_root + "\\" + release_index + "\\" + rel_name + "_fw.bin"
        shutil.copy2(fw_file_source,fw_file_target)
        #Get FS File, Copy it to release dir and rename it        
        fs_file_source = build_root + rel_name  + "\\littlefs.bin"
        fs_file_target = releases_root + "\\" + release_index + "\\" + rel_name + "_littlefs.bin"
        shutil.copy2(fs_file_source,fs_file_target)
        print(el)

