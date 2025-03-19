#DICOM to TXT file converter

#Author: A. Arzenton
#Contact: alberto.arzenton@pd.infn.it


from pydicom import dcmread
import numpy as np


image = dcmread("CT_1.dcm")
CT = image.pixel_array[0:147,0:159,0:159]

sh = [CT.shape[0], CT.shape[1], CT.shape[2]]

with open("CT_1.txt", "w") as output:
    lines = []
    for i in range(0, sh[2]):
        for j in range(0, sh[1]):
            for k in range(0, sh[0]):
                lines.append(str(k)+" "+str(j)+" "+str(i)+" "+str(CT[k,j,i])+'\n')
    output.writelines(lines)


image = dcmread("PET_18FDG_1.dcm")
CT = image.pixel_array[0:147,0:159,0:159]

sh = [CT.shape[0], CT.shape[1], CT.shape[2]]

with open("PET_1.txt", "w") as output:
    lines = []
    for i in range(0, sh[2]):
        for j in range(0, sh[1]):
            for k in range(0, sh[0]):
                lines.append(str(k)+" "+str(j)+" "+str(i)+" "+str(CT[k,j,i])+'\n')
    output.writelines(lines)
