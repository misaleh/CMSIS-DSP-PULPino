

arr = raw_input('Enter Q15 vector separated by spaces in hex decimal:   ')

arr = arr.split()
for l in arr:
    i = int(l,16)
    if(i > 0xFFFF):
       print l +'\033[91m' +' : Out of range!'
       continue
    if(i > 32767): #negative
        i =  i - 0x10000
        
    num = float(i)/float(32768)
    print num
