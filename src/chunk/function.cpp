#include <sstream>
#include <iomanip>
#include "function.h"
#include "serializer.h"
#include "visitor.h"
#include "elf/symbol.h"

void Function::serialize(ChunkSerializerOperations &op,
    ArchiveStreamWriter &writer) {

    writer.write(static_cast<uint64_t>(getAddress()));
    writer.writeAnyLength(getName());
}

bool Function::deserialize(ChunkSerializerOperations &op,
    ArchiveStreamReader &reader) {

    uint64_t address;
    std::string name;
    reader.read(address);
    reader.readAnyLength(name);

    setPosition(new AbsolutePosition(address));
    if(auto p = dynamic_cast<FuzzyFunction *>(this)) p->setName(name);

    return reader.stillGood();
}

void Function::accept(ChunkVisitor *visitor) {
    visitor->visit(this);
}

std::string FunctionFromSymbol::getName() const {
    return symbol->getName();
}

bool FunctionFromSymbol::hasName(std::string name) const {
    if(symbol->getName() == name) return true;
    for(auto s : getSymbol()->getAliases()) {
        if(std::string(s->getName()) == name) {
            return true;
        }
    }

    return false;
}

FuzzyFunction::FuzzyFunction(address_t originalAddress) {
    std::ostringstream stream;
    stream << "fuzzyfunc-0x" << std::hex << originalAddress;
    name = stream.str();
}

void FunctionList::accept(ChunkVisitor *visitor) {
    visitor->visit(this);
}

void FunctionList::serialize(ChunkSerializerOperations &op,
    ArchiveStreamWriter &writer) {

    op.serializeChildren(this, writer);
}

bool FunctionList::deserialize(ChunkSerializerOperations &op,
    ArchiveStreamReader &reader) {

    op.deserializeChildren(this, reader);
    return reader.stillGood();
}
