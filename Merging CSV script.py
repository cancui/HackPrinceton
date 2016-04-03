fout=open("out.csv","a")
# first file:
for line in open("sh1.csv"):
    fout.write(line)
# now the rest:    
for num in range(2,201):
    f = open("sh"+str(num)+".csv")
    f.next() # skip the header
    for line in f:
         fout.write(line)
    f.close() # not really needed
fout.close()