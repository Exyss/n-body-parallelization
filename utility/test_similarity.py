import csv 
import sys

tot_sum = 0

def similarity(str1, str2):
    result = 0

    for i in range(len(str1)-1):
        if float(str1[i]) == float(str2[i]): result += 100
        else: 
            if float(str1[i]) != 0 and float(str2[i]) != 0:
                result += min(float(str1[i]), float(str2[i]))*100/max(float(str1[i]), float(str2[i]))
    return result

if len(sys.argv) != 3:
    print("Correct usage: f'{sys.argv[1]}' <file1.csv> <file2.csv>")
else:
    with open(sys.argv[1], 'r') as file1:
        reader1 = csv.reader(file1)
        L1 = len(list(reader1))
        with open(sys.argv[2], 'r') as file2:
            reader2 = csv.reader(file2)
            L2 = len(list(reader2))
            mino = reader2
            LM = min(L1, L2)
            if L1 < L2: mino = reader1 
            file1.seek(0)
            file2.seek(0)
            for i in range(LM):
                tot_sum += similarity(next(reader1), next(reader2))
            file1.seek(0)
            file2.seek(0)  
            avg = tot_sum/(LM*(len(next(mino))-1))
    rowsdiff = abs(L1 - L2)
    print(f"somiglianza del: {avg}% con {rowsdiff} righe mancanti")

                
        
    