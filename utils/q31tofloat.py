

arr = raw_input('Enter Q31 vector separated by spaces in hex decimal:   ')

arr = arr.split()
for l in arr:
    i = int(l,16)
    if(i > 0xFFFFFFFF):
       print l +'\033[91m' +' : Out of range!'
       continue
    if(i > 0x7fffffff): #negative
        i =  i - 0x100000000
        
    num = float(i)/float(0x80000000)
    print num
