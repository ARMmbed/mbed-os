def print_section(start, end):
    print "[0x%08X - 0x%08X]" % (start, end)

def print_sections(h):
    start, last_address = None, None
    for a in h.addresses():
        if last_address is None:
            start, last_address = a, a
            continue
        
        if a > last_address + 1:
            print_section(start, last_address)
            start = a
        
        last_address = a
    
    if start:
        print_section(start, last_address)
