##  RADIONUCLIDE DOSIMETRY

Version 1.0

Geant4 application for internal dosimetry studies.
Developed within the ISOLPHARM project (https://isolpharm.pd.infn.it/web/).

Authors: A. Arzenton, D. Serafini, A. Leso, L. Morselli, M. Lunardon, A.
Andrighetto, G. Russo

Contact: alberto.arzenton@pd.infn.it, giorgio-russo@cnr.it


## Structure

This Geant4 application was developed for image-based internal dosimetry
calculations in the context of radionuclide therapy.
A virtual phantom is built taking the anatomical information contained in a
Computed Tomography (CT) file. Its geometry, composed by an array of voxels, is
based on G4PhantomParameterisation. Each tissue, represented by a different
density in the CT, can be associated to a different G4Material.
The generation of primaries follows instead the distribution contained in an
emission tomography file, which can be either a Positron Emission Tomography
(PET) or a Single-Photon Emission Computed Tomography (SPECT).
For every event, the deposited energy and the absorbed dose are scored in each
voxel which has been hit. At the end of the run, the final result is the
absorbed dose imparted by a cumulative activity corresponding to the total
number of events launched.


## DICOM conversion

The "DICOMconverter" folder contains a hybrid PET/CT study, with the DICOM files
"CT_1.dcm" and "PET_18FDG_1.dcm". Murine images were obtained after ¹⁸F-FDG
injection and are kindly provided by the group of Giorgio Russo from CNR-IBFM.
For the code to work properly, the DICOM files must be converted into TXT files
with the Python script "dcmtotxt.py" (make sure to have the "numpy" and
"pydicom" modules installed). Then, only PET (or SPECT) files must be converted
from TXT to ROOT format by using the ROOT macro "txttoroot.cc". To do that, from
bash type:

````bash
cd DICOMconverter
python3 dcmtotxt.py
root -q -l 'txttoroot.cc("<TXT file name>")'
````

Finally, the "hu.py" script can be useful to visualize the distribution of the
Hounsfield units (or other arbitrary units) in the CT files.


## How to run the example

After converting the DICOM files in the way just explained, from bash type:

````bash
mkdir build
cd build
cmake .. && make
./radionuclide_dosimetry
````

By doing this, the visualization macro "vis.mac" is automatically executed, as
specified in the main file; by default, only the phantom container is visualized
(and not all the voxels). Alternatively, "run.mac" can be used to skip the
visualization of the geometry and run a custom number of events:

````bash
./radionuclide_dosimetry macro/run.mac
````


## General instructions for the application

The source files ("src" folder) can be edited to change the main simulation
settings.
"DetectorConstruction.cc" reads the CT file for the construction of the virtual
phantom (line 170) and manages its composition in terms of G4Materials.
"PrimaryGeneratorAction.cc" selects the PET or SPECT file to sample the
probability distribution of the events (line 29) and sets which radionuclide to
generate (from line 46).
"RunAction.cc" creates and renames the output files (lines 53 and 54).
"StackingAction.cc" allows to kill certain particle species when generated by
the radioactive decay.
"VoxelSD.cc" neglects dose deposition in voxels made of air (line 57).
When changing the number of voxels and/or their size, it is important to update
these parameters in the header file "Parameters.hh", contained in the "include"
folder.


## How to read the results

The simulation output includes two ROOT files: a TNtuple providing all hit
information (event ID, voxel ID, deposited energy, absorbed dose) and a TH3F
containing only the 3D dose map.
The dose map can be visualized using Python-MatPlotLib; in this case, the
results must be first processed in ROOT with the macro "read_dose.cc", contained
in the "read" folder. Subsequently, the TXT file generated can be read by the
"dosemap_vis.py" script. With this, the 3D dose map can be visualized in the
three anatomical planes and compared to the fused DICOMs. From bash:

````bash
cd read
root -q -l 'read_dose.cc("<ROOT file name>")'
python3 dosemap_vis.py
````

Using instead the TNtuple, the relative Standard Error of the Mean (SEM) related
to the dose simulated in each voxel can be retrieved. Also in this case,
the output file must be processed in ROOT with the macro "read_relSEM" ("read"
folder). Again, the new TXT file can be read by a Python script,
"relsem_vis.py", which plots the 3D map of the relative SEM and prints its
average, minimum and maximum values. From bash:

````bash
cd read
root -q -l 'read_relSEM.cc("<ROOT file name>")'
python3 relsem_vis.py
````
