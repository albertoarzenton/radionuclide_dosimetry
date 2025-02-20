#ifndef VoxelHit_h
#define VoxelHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


class VoxelHit : public G4VHit
{
  public:

    VoxelHit();
    VoxelHit(const VoxelHit&);

    virtual ~VoxelHit();

    // Operators

     const VoxelHit& operator=(const VoxelHit&);
     G4int operator==(const VoxelHit&) const;

     inline void * operator new(size_t);
     inline void   operator delete(void*);

     // Methods from base class
     virtual void Draw(){}
     virtual void Print();

     // Methods to handle data
     void SetEdep(G4double);
     void SetDose(G4double);
     void SetCopyNumber(G4int);

     // Get methods
     G4double GetEdep();
     G4double GetDose();
     G4int    GetCopyNumber();

  private:

    G4double fEdep;
    G4double fDose;
    G4int fCopyNumber;

};

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

using VoxelHitsCollection = G4THitsCollection<VoxelHit>;

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

extern G4ThreadLocal G4Allocator<VoxelHit>* VoxelHitAllocator;

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

inline void* VoxelHit::operator new(size_t)
{
  if(!VoxelHitAllocator)
  {
    VoxelHitAllocator = new G4Allocator<VoxelHit>;
  }

  void *hit;

  hit = (void*) VoxelHitAllocator->MallocSingle();

  return hit;
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

inline void VoxelHit::operator delete(void *hit)
{
  if(!VoxelHitAllocator)
  {
    VoxelHitAllocator = new G4Allocator<VoxelHit>;
  }

  VoxelHitAllocator->FreeSingle((VoxelHit*)hit);
}

// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^
// ^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^


#endif
