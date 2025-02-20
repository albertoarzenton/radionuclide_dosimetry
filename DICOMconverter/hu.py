#Hounsfield units calibrator for CT imaging


#Import ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

from pydicom import dcmread
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

image = dcmread("CT_1.dcm") #CT
CT = image.pixel_array[0:132, 16:156, 12:152]


#Histogram ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ct = np.reshape(CT, CT.shape[0]*CT.shape[1]*CT.shape[2])
f, ax = plt.subplots()
nbins = 800
N, bins, patches = ax.hist(ct, bins=nbins)
ax.set_xlabel("Hounsfield units")
ax.set_ylabel("Counts")
ax.set_title("HU frequencies")
ax.set_ylim(0, 3000)
ax.set_xlim(-18000, 13000)

min = -17117
#max = 32766
tot = 49883
air_uplim = -3100
lung_uplim = -200
soft_uplim = 2900

for i in range(0, int((air_uplim-min)/tot*nbins)):
    patches[i].set_facecolor("silver")
for i in range(int((air_uplim-min)/tot*nbins), int((lung_uplim-min)/tot*nbins)):
    patches[i].set_facecolor("darkturquoise")
for i in range(int((lung_uplim-min)/tot*nbins), int((soft_uplim-min)/tot*nbins)):
    patches[i].set_facecolor("magenta")
for i in range(int((soft_uplim-min)/tot*nbins), nbins):
    patches[i].set_facecolor("gold")
handles = [Rectangle((0,0),1,1,color=c,ec="k") for c in ["silver", "darkturquoise", "magenta", "gold"]]
labels = ["Air", "Lung", "Soft tissue", "Bone"]
ax.legend(handles, labels)


#Plot ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

sh = CT.shape
c1, c2, c3 = int((sh[0]-1)/2), int((sh[1]-1)/2), int((sh[2]-1)/2) #Center
h1, h2, h3 = 1, 1.3, 1 #Move along the axes (center: h=1, range: [0,2])
A = 0.7 #Shade
mask = np.ma.masked_where(np.logical_or(CT < -200, CT > 2900), CT)**0

f0, ax0 = plt.subplots(1,3)
ax0[1].set_title("CT (0.5 mm), HU in [-200, 2900]")
ax0[0].imshow(mask[int(c1*h1)], "hsv")
p00 = ax0[0].imshow(CT[int(c1*h1)], "bone", alpha=A)
f0.colorbar(p00, ax=ax0[0])
ax0[1].imshow(mask[:,int(c2*h2)], "hsv")
p01 = ax0[1].imshow(CT[:,int(c2*h2)], "bone", alpha=A)
f0.colorbar(p01, ax=ax0[1])
ax0[2].imshow(mask[:,:,int(c3*h3)], "hsv")
p02 = ax0[2].imshow(CT[:,:,int(c3*h3)], "bone", alpha=A)
f0.colorbar(p02, ax=ax0[2], label="Hounsfield units")

ax0[0].set_xlabel("Transverse")
ax0[1].set_xlabel("Coronal")
ax0[2].set_xlabel("Sagittal")
plt.show()


#Predictions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#[0, 4000] Air
#[4000, 6000] Lungs
#[6000, 11000] Soft tissue
#[11000, 65535] Bone

#[9000, 11000] Urinary bladder
#[10000, 12000] Instrumentation

#Geant4 provides: soft, bone, lung, muscle, brain

#To consider: urinary bladder, lungs, brain, heart
