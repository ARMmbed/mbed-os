import os
import re




def replace_header(in_path):
    with open("copyright_header.txt", "r") as fd:
        header = fd.read()

        for root, dirs, files in os.walk(in_path):
            for fn in [os.path.join(root, x) for x in files]:
                with open(fn, "r+") as fd:
                    #print fn
                    s = fd.read()
                    start = s.find("/*")
                    end = s.find("*/")
                    copyright_str = s[start:end+2]
                    if ("copyright (c)" in copyright_str.lower()) and ("nordic semiconductor" in copyright_str.lower()):

                    
                        pat_reg  = r"copyright \(c\)[\s]{1,5}([0-9]{4})"
                        find_obj = re.search(pat_reg, copyright_str.lower())
                        
                        if find_obj == None:
                            header_year = re.sub(r'(<YEAR>)',"2000",header)
                            print "default date>>" + fn
                        else:
                            header_year = re.sub(r'(<YEAR>)',find_obj.group(1),header)
                        
                        s = s.replace(copyright_str, header_year)
                        
                        #print(header_year)

                        fd.seek(0)
                        fd.write(s)
                        fd.truncate()
                        
                        ##print "+"*35
                        print fn


pathes = ["..\\sdk",
          "..\\TARGET_MCU_NRF52832\\sdk",
          "..\\TARGET_MCU_NRF52840\\sdk"]
    
for path in pathes:
    replace_header(path)
