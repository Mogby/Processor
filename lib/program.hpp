#ifndef NCPU_PROGRAM_HPP
#define NCPU_PROGRAM_HPP

#include "instruction.hpp"
#include "types.hpp"

#include <unordered_set>
#include <vector>

namespace NCPU {

class TProgram {
  public:
    void ReadFromAsmFile(const TString& filename);
    void WriteToAsmFile(const TString& filename) const;
    void ReadFromBinaryFile(const TString& filename);
    void WriteToBinaryFile(const TString& filename) const;
    void Run();

  private:
    void Wipe();
    void RemoveComment(TString& assembly_line) const;
    void ReplaceLabels(TString& assembly_line) const;

  private:
    std::vector<IInstructionPtr> instructions_;
    std::unordered_map<TString, TSize> label_offsets_;
};

} // namespace NCPU

#endif // NCPU_PROGRAM_HPP