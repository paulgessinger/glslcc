#ifndef GLSLCC_INCLUDER_HPP
#define GLSLCC_INCLUDER_HPP

#include "SPIRV/GlslangToSpv.h"

#include "sx/allocator.h"
#include "sx/os.h"

// Includer
class Includer : public glslang::TShader::Includer {
public:
  virtual ~Includer() {}

  IncludeResult* includeSystem(const char* headerName,
                               const char* includerName,
                               size_t inclusionDepth) override
  {
    for (auto i = m_systemDirs.begin(); i != m_systemDirs.end(); ++i) {
      std::string header_path(*i);
      if (header_path.back() != '/')
        header_path += "/";
      header_path += headerName;

      auto sr = sx_os_stat(header_path.c_str()).type;

      if (sx_os_stat(header_path.c_str()).type == SX_FILE_TYPE_REGULAR) {
        sx_mem_block* mem = sx_file_load_bin(g_alloc, header_path.c_str());
        if (mem) {
          return new (sx_malloc(g_alloc, sizeof(IncludeResult)))
              IncludeResult(header_path, (const char*)mem->data, (size_t)mem->size, mem);
        }
      }
    }
    return nullptr;
  }

  IncludeResult* includeLocal(const char* headerName,
                              const char* includerName,
                              size_t inclusionDepth) override
  {
    char cur_dir[256];
    sx_os_path_pwd(cur_dir, sizeof(cur_dir));
    std::string header_path(cur_dir);
    std::replace(header_path.begin(), header_path.end(), '\\', '/');
    if (header_path.back() != '/')
      header_path += "/";
    header_path += headerName;

    sx_mem_block* mem = sx_file_load_bin(g_alloc, header_path.c_str());
    if (mem) {
      return new (sx_malloc(g_alloc, sizeof(IncludeResult)))
          IncludeResult(header_path, (const char*)mem->data, (size_t)mem->size, mem);
    }
    return nullptr;
  }

  // Signals that the parser will no longer use the contents of the
  // specified IncludeResult.
  void releaseInclude(IncludeResult* result) override
  {
    if (result) {
      sx_mem_block* mem = (sx_mem_block*)result->userData;
      if (mem)
        sx_mem_destroy_block(mem);
      result->~IncludeResult();
      sx_free(g_alloc, result);
    }
  }

  void addSystemDir(const char* dir)
  {
    std::string std_dir(dir);
    std::replace(std_dir.begin(), std_dir.end(), '\\', '/');
    m_systemDirs.push_back(std_dir);
  }

  void addIncluder(const Includer& includer)
  {
    for (const std::string& inc : includer.m_systemDirs) {
      m_systemDirs.push_back(inc);
    }
  }

private:
  std::vector<std::string> m_systemDirs;
};

#endif // GLSLCC_INCLUDER_HPP
