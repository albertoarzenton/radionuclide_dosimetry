#Dose map, CT and PET or SPECT visualization

#Author: A. Arzenton
#Contact: alberto.arzenton@pd.infn.it


#Import ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from pydicom import dcmread
import numpy as np
import matplotlib.pyplot as plt

image = dcmread("../DICOMconverter/CT_1.dcm") #CT
CT = image.pixel_array[0:132, 16:156, 12:152] #[0:148, 0:159, 0:159] full image

# image = dcmread("../DICOMconverter/PET_18FDG_1.dcm") #PET or SPECT
# img = image.pixel_array[0:132, 16:156, 12:152]


#Parameters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

sh = CT.shape
c1, c2, c3 = int((sh[0]-1)/2), int((sh[1]-1)/2), int((sh[2]-1)/2) #Center


#Geant4 results ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

with open("dose_map.txt",'r') as gatefile:
    lines = gatefile.readlines()
    dose = []
    slice = []

    for l in lines:
        slice.append( [ float(a) for a in l.split() ] )

        if(slice[-1] == []) :
            slice.pop()
            dose.append(slice)
            slice = []

    dose = np.array(dose)[0:132, 16:156, 12:152]

dose = dose*1000 #From Gy to mGy
events = 1e5 #Simulated events
TCA = 4.8e8 #Total cumulated activity [Bq·s]
dose = dose/events*TCA #Normalization to total cumulated activity
#print(dose.shape)


#Visualization ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

h1, h2, h3 = 1, 1.3, 1 #Move along the axes (center: h=1, range: [0,2])
A = 0.8 #Dose, PET or SPECT shade
max_tra = np.max([np.max(dose[int(c1*h1)]), np.max(dose[int(c1*h1)])])
max_cor = np.max([np.max(dose[:,int(c2*h2)]), np.max(dose[:,int(c2*h2)])])
max_sag = np.max([np.max(dose[:,:,int(c3*h3)]), np.max(dose[:,:,int(c3*h3)])])

f, ax = plt.subplots(1,3)

#for i in range(0, 2):
ax[0].imshow(CT[int(c1*h1)], "bone")
ax[1].imshow(CT[:,int(c2*h2)], "bone")
ax[2].imshow(CT[:,:,int(c3*h3)], "bone")

# pos00 = ax[0,0].imshow(img[int(c1*h1)], "inferno", alpha=A)
# f.colorbar(pos00, ax=ax[0,0])
# pos01 = ax[0,1].imshow(img[:,int(c2*h2)], "inferno", alpha=A)
# f.colorbar(pos01, ax=ax[0,1])
# pos02 = ax[0,2].imshow(img[:,:,int(c3*h3)], "inferno", alpha=A)
# f.colorbar(pos02, ax=ax[0,2], label="[kBq/mL]")
# ax[0,1].set_title("F-18 PET (0.5 mm)")

pos10 = ax[0].imshow(dose[int(c1*h1)], "magma", vmax=max_tra, alpha=A)
f.colorbar(pos10, ax=ax[0])
pos11 = ax[1].imshow(dose[:,int(c2*h2)], "magma", vmax=max_cor, alpha=A)
f.colorbar(pos11, ax=ax[1])
pos12 = ax[2].imshow(dose[:,:,int(c3*h3)], "magma", vmax=max_sag, alpha=A)
f.colorbar(pos12, ax=ax[2], label="[mGy]")
f.suptitle("Dose map")

ax[0].set_xlabel("Transverse")
ax[1].set_xlabel("Coronal")
ax[2].set_xlabel("Sagittal")

plt.show()
