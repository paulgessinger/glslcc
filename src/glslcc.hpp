#ifndef GLSLCC_GLSLCC_HPP
#define GLSLCC_GLSLCC_HPP

#include "SPIRV/GlslangToSpv.h"

#include <memory>

enum shader_lang {
  SHADER_LANG_GLES = 0,
  SHADER_LANG_HLSL,
  SHADER_LANG_MSL,
  SHADER_LANG_GLSL,
  SHADER_LANG_COUNT
};

struct p_define {
  char* def;
  char* val;
};

enum output_error_format {
  OUTPUT_ERRORFORMAT_GLSLANG = 0,
  OUTPUT_ERRORFORMAT_MSVC,
  OUTPUT_ERRORFORMAT_GCC
};

class Includer;

struct cmd_args {
  const char* vs_filepath;
  const char* fs_filepath;
  const char* cs_filepath;
  const char* out_filepath;
  shader_lang lang;
  p_define* defines;
  std::unique_ptr<Includer> includer;
  int profile_ver;
  int invert_y;
  int preprocess;
  int flatten_ubos;
  int sgs_file;
  int reflect;
  int compile_bin;
  int debug_bin;
  int optimize;
  int silent;
  int validate;
  output_error_format err_format;
  const char* cvar;
  const char* reflect_filepath;
};

int compile_files(cmd_args& args, const TBuiltInResource& limits_conf);

#endif //GLSLCC_GLSLCC_HPP
