#ifndef EGALITO_PASS_RELOCDATA_H
#define EGALITO_PASS_RELOCDATA_H

#include "chunkpass.h"
#include "elf/reloc.h"

class ElfMap;
class ElfSpace;
class Conductor;

class FindAnywhere {
private:
    Conductor *conductor;
    ElfSpace *elfSpace;
    Function *found;
public:
    FindAnywhere(Conductor *conductor, ElfSpace *elfSpace)
        : conductor(conductor), elfSpace(elfSpace) {}

    /** Tries to resolve the address that the named entity lives at.
        On success, sets address and returns true. On failure, returns false.
    */
    bool resolveName(const char *name, address_t *address);
    bool resolveObject(const char *name, address_t *address, size_t *size);
private:
    bool resolveNameHelper(const char *name, address_t *address,
        ElfSpace *space);
    bool resolveObjectHelper(const char *name, address_t *address,
        size_t *size, ElfSpace *space);
};

/** Fixes relocations in the data section prior to running code.

    This must be called after all libraries have been parsed and contain
    a FunctionAliasMap.
*/
class RelocDataPass : public ChunkPass {
private:
    ElfMap *elf;
    ElfSpace *elfSpace;
    RelocList *relocList;
    Conductor *conductor;
    Module *module;
public:
    RelocDataPass(ElfMap *elf, ElfSpace *elfSpace, RelocList *relocList,
        Conductor *conductor)
        : elf(elf), elfSpace(elfSpace), relocList(relocList),
        conductor(conductor) {}
    virtual void visit(Module *module);
    virtual void visit(Instruction *instruction) {}
private:
    void fixRelocation(Reloc *r);
};

#endif