import sys

#Function that take a string of length 3
#and convert it to a Base64 string of length 4
def threeCharToBase64Encode(s):
    buffer='';
    #convert each char to binary and put into a buffer
    for c in s:
        ascii=(bin(ord(c))[2:]).zfill(8);
        buffer=buffer+ascii;
    ##print buffer
    #Cut the whole buffer into 4 pieces and convert each piece back to decimal
    newlst=list(int(buffer[6*i:6*i+6],2) for i in range(len(buffer)/6))
    res=[]
    #map the decimal value to the corresponding symbol in base64 table
    for i in newlst:
        if i <26:
            res.append(chr(i+65))
        elif i<52:
            res.append(chr(i+97-26))
        elif i<62:
            res.append(chr(i-52+48))
        elif i==62:
            res.append('+')
        elif i==63:
            res.append('/')
        else:
            print 'error';
            exit();
    return ''.join(res);
    

#Read the input from file
filename=sys.argv[-1]
#filename="Q3.in"
f=open(filename,'r')
s=f.read()
f.close()

#Encode the input string 3 char by 3 char, put the result into output buffer
outBuffer=''
for i in range(len(s)/3):
    threeChar=(s[3*i:3*i+3])
    outBuffer=outBuffer+threeCharToBase64Encode(threeChar);


#Write the output to the .out file
f=open("Q3.out",'w')
f.write(outBuffer)
f.close()

print "Original string="+s
print "Encoded string="+outBuffer
