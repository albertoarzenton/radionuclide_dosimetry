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


image1 = dcmread("PET_18FDG_1.dcm")
PET1 = 1.0*image1.pixel_array[0:147,0:159,0:159]
image2 = dcmread("PET_18FDG_1.dcm")
PET2 = 1.0*image2.pixel_array[0:147,0:159,0:159]
image3 = dcmread("PET_18FDG_1.dcm")
PET3 = 1.0*image3.pixel_array[0:147,0:159,0:159]

sh1 = [PET1.shape[0], PET1.shape[1], PET1.shape[2]]

def trapezoidal(a,b,c): #Trapezoidal integration of the activity
    t = [1,4,24]*3600 #PET or SPECT acquisition time after injection [s]
    return (a*t[0] + (b+a)*(t[1]-t[0]) + (c+b)*(t[2]-t[1]))/2 #Cumulated activity

with open("PET_1.txt", "w") as output:
    lines = []
    TCA = 0 #Total cumulated activity [kBq/mL]
    for i in range(0, sh1[2]):
        for j in range(0, sh1[1]):
            for k in range(0, sh1[0]):
                TCA += trapezoidal(PET1[k,j,i],PET2[k,j,i],PET3[k,j,i])
                lines.append(str(k)+" "+str(j)+" "+str(i)+" "+str(trapezoidal(PET1[k,j,i],PET2[k,j,i],PET3[k,j,i]))+'\n')
    output.writelines(lines)

vs = 0.5 #Voxel size [mm]
vv = vs**3/1e3 #Voxel volume [mL]
print("Total cumulated activity:", TCA*vv*1e3, "Bq·s")
