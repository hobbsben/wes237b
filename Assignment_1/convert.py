
# opens a .bin file called "fm_output" and converts it's contents to a .txt file
with open('fm_output') as fin:
	buf = fin.read()
bytes = map(ord,buf)

# print bytes[:10]  # prints to console

signal = bytes[:100]

f = open("test3.txt","w")
print >> f, signal
f.close()