#!/bin/bash

printf "#ifndef MYUPLAY_MYENGINE_SHADERCHUNK\n" > ShaderChunk.hpp
printf "#define MYUPLAY_MYENGINE_SHADERCHUNK\

#include <string>

namespace MyUPlay {\n\
namespace MyEngine {\n\
namespace ShaderChunk {\n" >> ShaderChunk.hpp

for f in *.glsl
do

	printf "const std::string ${f::-5} =" >> ShaderChunk.hpp

	while read -r line || [[ -n "$line" ]]; do
		printf "\n\"$line\\\\n\"" >> ShaderChunk.hpp
	done < "$f"

	printf ";\n" >> ShaderChunk.hpp

done

printf "}}}\n#endif\n" >> ShaderChunk.hpp

