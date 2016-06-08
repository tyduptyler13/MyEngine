#!/bin/bash

printf "#ifndef MYUPLAY_MYENGINE_SHADERCHUNK
#define MYUPLAY_MYENGINE_SHADERCHUNK

namespace MyUPlay {\n
namespace MyEngine {\n
namespace ShaderChunk {\n" > ShaderChunk.hpp

for f in *.glsl
do

	printf "extern const char* ${f::-5};\n" >> ShaderChunk.hpp

done

printf "}\n}\n}\n#endif\n" >> ShaderChunk.hpp

printf "#include \"ShaderChunk/ShaderChunk.hpp\"

namespace MyUPlay {\n
namespace MyEngine {\n
namespace ShaderChunk {\n" > ../../src/ShaderChunk.cpp

for f in *.glsl
do

	printf "const char* ${f::-5} =" >> ../../src/ShaderChunk.cpp

	while read -r line || [[ -n "$line" ]]; do
		printf "\n\"$line\\\\n\"" >> ../../src/ShaderChunk.cpp
	done < "$f"

	printf ";\n" >> ../../src/ShaderChunk.cpp

done

printf "}\n}\n}\n" >> ../../src/ShaderChunk.cpp
