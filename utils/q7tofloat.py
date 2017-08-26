

arr = raw_input('Enter Q7 vector separated by spaces in hex decimal:   ')

arr = arr.split()
for l in arr:
    i = int(l,16)
    if(i > 0xFF):
       print l +'\033[91m' +' : Out of range!'
       continue
    if(i > 127): #negative
        i =  i - 0x100 ;
        
    num = float(i)/float(128)
    print num
