#Relative Standard Error of the Mean (SEM) per voxel visualization

#Author: A. Arzenton
#Contact: alberto.arzenton@pd.infn.it


#Import ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from pydicom import dcmread
import numpy as np
import matplotlib.pyplot as plt

image = dcmread("../DICOMconverter/CT_1.dcm") #CT
CT = image.pixel_array[0:132, 16:156, 12:152] #[0:148, 0:159, 0:159] full image


#Parameters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

a = 0.5 #Voxel size in mm

sh = CT.shape
c1, c2, c3 = int((sh[0]-1)/2), int((sh[1]-1)/2), int((sh[2]-1)/2) #Center


#Geant4 results ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

with open("relSEM_map.txt",'r') as gatefile:
    lines = gatefile.readlines()
    relsem = []
    slice = []

    for l in lines:
        slice.append( [ float(a) for a in l.split() ] )

        if(slice[-1] == []) :
            slice.pop()
            relsem.append(slice)
            slice = []

    relsem = np.array(relsem)[0:132, 16:156, 12:152]

#print(relsem.shape)
relsemi = np.zeros((sh[1],sh[2]))
for j in range(0, sh[1]):
    for k in range(0, sh[2]):
        relsemi[j,k] = np.max(relsem[:,j,k])
relsemj = np.zeros((sh[0],sh[2]))
for i in range(0, sh[0]):
    for k in range(0, sh[2]):
        relsemj[i,k] = np.max(relsem[i,:,k])
relsemk = np.zeros((sh[0],sh[1]))
for i in range(0, sh[0]):
    for j in range(0, sh[1]):
        relsemk[i,j] = np.max(relsem[i,j,:])


#Stats

relsem = np.ma.masked_where(relsem==0, relsem)  #Voxels with at least one hit
print("Average relative SEM: ", np.mean(relsem),"%")
print("Minimum relative SEM: ", np.min(relsem),"%")
print("Maximum relative SEM: ", np.max(relsem),"%")


#Visualization ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

h1, h2, h3 = 1, 1.3, 1 #Move along the axes (center: h=1, range: [0,2])
A = 0.8 #relsem, PET or SPECT shade
max_tra = np.max([np.max(relsem[int(c1*h1)]), np.max(relsem[int(c1*h1)])])
max_cor = np.max([np.max(relsem[:,int(c2*h2)]), np.max(relsem[:,int(c2*h2)])])
max_sag = np.max([np.max(relsem[:,:,int(c3*h3)]), np.max(relsem[:,:,int(c3*h3)])])

f, ax = plt.subplots(1,3)

ax[0].imshow(CT[int(c1*h1)], "bone")
ax[1].imshow(CT[:,int(c2*h2)], "bone")
ax[2].imshow(CT[:,:,int(c3*h3)], "bone")

pos0 = ax[0].imshow(relsem[int(c1*h1)], "viridis", vmax=max_tra, alpha=A)
f.colorbar(pos0, ax=ax[0])
pos1 = ax[1].imshow(relsem[:,int(c2*h2)], "viridis", vmax=max_cor, alpha=A)
f.colorbar(pos1, ax=ax[1])
pos2 = ax[2].imshow(relsem[:,:,int(c3*h3)], "viridis", vmax=max_sag, alpha=A)
f.colorbar(pos2, ax=ax[2], label="Relative standard error of the mean [%]")
plt.suptitle("Relative SEM map")

ax[0].set_xlabel("Transverse")
ax[1].set_xlabel("Coronal")
ax[2].set_xlabel("Sagittal")

plt.show()
